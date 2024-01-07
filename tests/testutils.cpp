#include "testutils.hpp"

#include <memory>
namespace test_utils {

SignalCatcher::SignalCatcher(const SignalCatcherParams &params) {
  m_excludes = params.excludes;
  if (params.exclude_id) {
    m_excludes.insert("id");
    m_excludes.insert("__typeName");
  }
  auto mo = params.source_obj->metaObject();
  for (int i = mo->propertyOffset(); i < mo->propertyCount(); ++i) {
    auto property = mo->property(i);
    QString prop_name = property.name();
    if (!m_excludes.contains(prop_name)) {
      REQUIRE(property.hasNotifySignal());
      if (params.only.has_value() && params.only.value() != prop_name) {
        continue;
      }
      auto spy = new QSignalSpy(params.source_obj, property.notifySignal());
      if (!spy->isValid()) {
        throw std::runtime_error("spy for property " + prop_name.toStdString() +
                                 " is not valid");
      }
      m_spys.emplace_front(spy, prop_name);
    }
  }
  if (params.only.has_value() && m_spys.empty()) {
    if (m_excludes.contains(params.only.value())) {
      throw std::runtime_error(params.only.value().toStdString() +
                               " is excluded and cannot be included");
    }
    throw std::runtime_error("could not find property signal for " +
                             params.only.value().toStdString());
  }
};
/*
 * Wait for signals emission, returns true if all included
 * signals were caught.
 */
bool SignalCatcher::wait(int timeout) {
  for (const auto &spy_pair : m_spys) {
    if (!QTest::qWaitFor([&]() -> bool { return spy_pair.first->count() != 0; },
                         timeout)) {
      qDebug() << "Signal " << spy_pair.second << " wasn't caught.";
      return false;
    }
    qDebug() << spy_pair.second.toStdString()
             << "was fired:" << spy_pair.first->count() << " times.";
  };
  return true;
}

}; // namespace test_utils
