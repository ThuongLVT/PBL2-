#include "LoginDialog.h"
#include "Core/QuanLy/HeThongQuanLy.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QScreen>
#include <QGuiApplication>
#include <QAction>
#include <QIcon>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent), m_isAdmin(false)
{
    setupUI();
    applyStyles();

    // Set window flags to remove title bar
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // Set fixed size for dialog window - 1200x800 for cropped image
    setFixedSize(1200, 800); // Kích thước cố định theo ảnh đã cắt

    // Center on screen
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
}
LoginDialog::~LoginDialog()
{
}

void LoginDialog::setupUI()
{
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Create central widget for login form (will be centered) - Compact version
    QWidget *centerWidget = new QWidget(this);
    centerWidget->setObjectName("loginFormContainer");
    centerWidget->setFixedSize(450, 450); // Tăng thêm kích thước login form

    QVBoxLayout *formLayout = new QVBoxLayout(centerWidget);
    formLayout->setSpacing(15);
    formLayout->setContentsMargins(30, 30, 30, 30);

    // Title
    m_titleLabel = new QLabel("ĐĂNG NHẬP", centerWidget);
    m_titleLabel->setObjectName("loginTitle");
    m_titleLabel->setAlignment(Qt::AlignCenter);

    // Username - NO LABEL, only input with placeholder
    m_usernameEdit = new QLineEdit(centerWidget);
    m_usernameEdit->setObjectName("loginInput");
    m_usernameEdit->setPlaceholderText("Tên đăng nhập");
    m_usernameEdit->setFixedHeight(40);
    m_usernameEdit->setText("admin"); // Set default username

    // Password - NO LABEL, only input with placeholder
    m_passwordEdit = new QLineEdit(centerWidget);
    m_passwordEdit->setObjectName("loginInput");
    m_passwordEdit->setPlaceholderText("Mật khẩu");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setFixedHeight(40);

    // Add toggle password visibility action
    QAction *togglePasswordAction = m_passwordEdit->addAction(QIcon(":/icons/eye-off.svg"), QLineEdit::TrailingPosition);
    connect(togglePasswordAction, &QAction::triggered, this, [this, togglePasswordAction]()
            {
        if (m_passwordEdit->echoMode() == QLineEdit::Password) {
            m_passwordEdit->setEchoMode(QLineEdit::Normal);
            togglePasswordAction->setIcon(QIcon(":/icons/eye.svg"));
        } else {
            m_passwordEdit->setEchoMode(QLineEdit::Password);
            togglePasswordAction->setIcon(QIcon(":/icons/eye-off.svg"));
        } });

    // Buttons
    m_loginButton = new QPushButton("Đăng nhập", centerWidget);
    m_loginButton->setObjectName("loginButton");
    m_loginButton->setFixedHeight(40);
    m_loginButton->setCursor(Qt::PointingHandCursor);
    m_loginButton->setDefault(true); // Set as default button to handle Enter key

    m_closeButton = new QPushButton("Đóng", centerWidget);
    m_closeButton->setObjectName("closeButton");
    m_closeButton->setFixedHeight(50);
    m_closeButton->setCursor(Qt::PointingHandCursor);

    // Add widgets to form layout - NO LABELS
    formLayout->addStretch();
    formLayout->addWidget(m_titleLabel);
    formLayout->addSpacing(20);
    formLayout->addWidget(m_usernameEdit);
    formLayout->addWidget(m_passwordEdit);
    formLayout->addSpacing(15);
    formLayout->addWidget(m_loginButton);
    formLayout->addSpacing(10); // Add spacing between login and close buttons
    formLayout->addWidget(m_closeButton);
    formLayout->addStretch();

    // Add center widget to main layout (centered)
    mainLayout->addStretch();
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(centerWidget);
    hLayout->addStretch();
    mainLayout->addLayout(hLayout);
    mainLayout->addStretch();

    // Connect signals
    connect(m_loginButton, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(m_closeButton, &QPushButton::clicked, this, &LoginDialog::onCloseClicked);
    // Removed returnPressed connection to avoid double triggering with default button
    // connect(m_passwordEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);

    // Set focus to password field by default
    m_passwordEdit->setFocus();
}

void LoginDialog::applyStyles()
{
    QString styles = R"(
        QWidget#loginFormContainer {
            background-color: rgba(0, 0, 0, 180);
            border-radius: 15px;
        }
        
        QLabel#loginTitle {
            color: white;
            font-size: 32px;
            font-weight: bold;
            letter-spacing: 2px;
        }
        
        QLabel#loginLabel {
            color: #e5e7eb;
            font-size: 14px;
            font-weight: 500;
        }
        
        QLineEdit#loginInput {
            background-color: rgba(255, 255, 255, 200);
            border: 2px solid #4ade80;
            border-radius: 8px;
            padding: 0 15px;
            font-size: 15px;
            color: #1f2937;
        }
        
        QLineEdit#loginInput:focus {
            background-color: white;
            border: 2px solid #16a34a;
        }
        
        QPushButton#loginButton {
            background-color: #16a34a;
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 16px;
            font-weight: bold;
        }
        
        QPushButton#loginButton:hover {
            background-color: #15803d;
        }
        
        QPushButton#loginButton:pressed {
            background-color: #14532d;
        }
        
        QPushButton#closeButton {
            background-color: rgba(239, 68, 68, 200);
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 16px;
            font-weight: bold;
        }
        
        QPushButton#closeButton:hover {
            background-color: rgba(220, 38, 38, 220);
        }
        
        QPushButton#closeButton:pressed {
            background-color: rgba(185, 28, 28, 240);
        }
    )";

    setStyleSheet(styles);
}

void LoginDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // Draw background image - crop 100px from top and bottom (1200x1000 -> 1200x800)
    QPixmap bgImage(":/images/peakpx.jpg");
    if (!bgImage.isNull())
    {
        // Crop image: remove 100px from top and 100px from bottom
        // Original: 1200x1000, After crop: 1200x800
        int cropTop = 100;
        int cropBottom = 125;
        int croppedHeight = bgImage.height() - cropTop - cropBottom;

        QPixmap croppedImage = bgImage.copy(
            0,               // x: start from left
            cropTop,         // y: skip top 100px
            bgImage.width(), // width: full width (1200)
            croppedHeight    // height: 1000 - 100 - 100 = 800
        );

        // Scale cropped image to fit window (1200x800)
        QPixmap scaledBg = croppedImage.scaled(
            width(),
            height(),
            Qt::KeepAspectRatio,     // Keep aspect ratio
            Qt::SmoothTransformation // High quality scaling
        );

        // Center the scaled image
        int x = (width() - scaledBg.width()) / 2;
        int y = (height() - scaledBg.height()) / 2;

        // Debug info (only print once)
        static bool printed = false;
        if (!printed)
        {
            qDebug() << "================================";
            qDebug() << "Window size:" << width() << "x" << height();
            qDebug() << "Original image:" << bgImage.width() << "x" << bgImage.height();
            qDebug() << "Cropped image:" << croppedImage.width() << "x" << croppedImage.height();
            qDebug() << "Scaled image:" << scaledBg.width() << "x" << scaledBg.height();
            qDebug() << "Image position: (" << x << "," << y << ")";
            qDebug() << "================================";
            printed = true;
        }

        // Draw scaled image
        painter.drawPixmap(x, y, scaledBg);
    }
    else
    {
        qDebug() << "ERROR: Background image not found!";
        // Fallback: gradient background
        QLinearGradient gradient(0, 0, width(), height());
        gradient.setColorAt(0, QColor(22, 163, 74)); // #16a34a
        gradient.setColorAt(1, QColor(20, 83, 45));  // #14532d
        painter.fillRect(rect(), gradient);
    }
}

void LoginDialog::onLoginClicked()
{
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();

    if (username.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập đầy đủ thông tin!");
        return;
    }

    if (authenticate(username, password))
    {
        m_username = username;
        accept();
    }
    else
    {
        QMessageBox::critical(this, "Lỗi", "Tên đăng nhập hoặc mật khẩu không đúng!");
        m_passwordEdit->clear();
        m_passwordEdit->setFocus();
    }
}

void LoginDialog::onCloseClicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Xác nhận",
        "Bạn có chắc chắn muốn đóng ứng dụng?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        reject(); // Close dialog and exit application
        qApp->quit();
    }
}

bool LoginDialog::authenticate(const QString &username, const QString &password)
{
    HeThongQuanLy *sys = HeThongQuanLy::getInstance();
    QuanLyNhanVien *staffMgr = sys->layQuanLyNhanVien();

    // Check admin login from CSV
    QuanTriVien *admin = staffMgr->timQuanTriVienTheoUsername(username.toStdString());
    if (admin && admin->dangNhap(password.toStdString()))
    {
        m_isAdmin = true;
        return true;
    }

    // Check staff login from CSV
    NhanVien *staff = staffMgr->timNhanVienTheoUsername(username.toStdString());
    if (staff && staff->dangNhap(password.toStdString()))
    {
        m_isAdmin = false;
        return true;
    }

    return false;
}
