#pragma once

#include <QSignalSpy>
#include <QTest>

#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickView>

#include "testframework.hpp"
#include <filesystem>
#include <memory>

namespace fs = std::filesystem;

#define you_assert_m(cond, msg)                                                \
  if (!cond) {                                                                 \
    qDebug() << msg;                                                           \
  }                                                                            \
  REQUIRE(cond);

namespace test_utils {

struct ModelSignalSpy {
  QSignalSpy *about_to;
  QSignalSpy *after;
  explicit ModelSignalSpy(QSignalSpy *about, QSignalSpy *_after)
      : about_to{about}, after{_after} {
    REQUIRE(about->isEmpty());
    REQUIRE(after->isEmpty());
  }

  void validate() {
    REQUIRE(!about_to->isEmpty());
    REQUIRE(!after->isEmpty());
  }
};

#define YOU_STRINGIFY(x) #x
#define YOU_TOSTRING(x) YOU_STRINGIFY(x)
struct QmlBot {
  QQuickView *m_qquick_view;
  QmlBot() {
    m_qquick_view = new QQuickView();
#ifdef _WIN32
    auto additional_path =
        QDir(fs::path(YOU_TOSTRING(TESTS_QML_DIR))).absolutePath();
    additional_path.replace('/', '\\');
    if (!QDir(additional_path).exists())
      qDebug() << "additional qml path doesn't exist";
    m_qquick_view->engine()->addImportPath(additional_path);

#endif
    auto qmltester =
        QFileInfo(fs::path(__FILE__).parent_path() / "qmltester.qml");
    m_qquick_view->setSource(QUrl::fromLocalFile(qmltester.absoluteFilePath()));
    auto loaded = QTest::qWaitFor(
        [this] { return this->m_qquick_view->status() == QQuickView::Ready; });
    auto errors = m_qquick_view->errors();
    if (!errors.empty() || !loaded) {
      qDebug() << errors;
      throw "errors during qml load";
    }
    m_qquick_view->show();
  }

  template <typename T> T find(const QString &objectName) {
    return m_qquick_view->findChild<T>(objectName);
  };

  QQuickItem *root_loader() { return find<QQuickItem *>("rootLoader"); };

  QQuickItem *load(const fs::path &path) {
    auto source = QFileInfo(path);
    you_assert_m(source.exists(),
                 "source qml file doesn't exists.") auto component =
        new QQmlComponent(m_qquick_view->engine(),
                          QUrl::fromLocalFile(source.absoluteFilePath()),
                          QQmlComponent::PreferSynchronous);

    QObject *object = component->create();
    m_qquick_view->rootObject()
        ->findChild<QQuickItem *>("rootLoader")
        ->setProperty("sourceComponent",
                      QVariant::fromValue(qobject_cast<QObject *>(component)));
    object->setParent(m_qquick_view->rootObject());
    auto errors = m_qquick_view->errors();

    if (!errors.empty()) {
      qDebug() << errors << m_qquick_view->errors();
      throw "errors during qml load";
    }
    auto ret = root_loader()->findChildren<QQuickItem *>()[0];
    return ret;
  }

  [[nodiscard]] QQmlEngine *engine() const { return m_qquick_view->engine(); }

  ~QmlBot() {
    m_qquick_view->close();
    delete m_qquick_view;
  }
};
struct SignalCatcherParams {
  const QObject *source_obj = nullptr;
  const QSet<QString> &excludes = {};
  bool exclude_id = true;
  const std::optional<QString> &only = {};
};

class SignalCatcher {
  std::list<std::pair<QSignalSpy *, QString>> m_spys = {};
  QSet<QString> m_excludes = {};

public:
  explicit SignalCatcher(const SignalCatcherParams &params);

  [[nodiscard]] bool wait(int timeout = 1000);
};
}; // namespace test_utils
