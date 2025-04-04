/*
   SPDX-FileCopyrightText: 2003-2007 Clarence Dang <dang@kde.org>
   SPDX-FileCopyrightText: 2015, 2016 Martin Koller <kollix@aon.at>

   SPDX-License-Identifier: BSD-2-Clause
*/

#include <KAboutData>

#include "kpVersion.h"
#include "mainWindow/kpMainWindow.h"
#include <document/kpDocument.h>
#include <kolourpaintlicense.h>

#include <KCrash>
#include <KLocalizedString>
#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QImageReader>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QImageReader::setAllocationLimit(0); // no explicit memory limit

    KLocalizedString::setApplicationDomain("kolourpaint");

    KAboutData aboutData(QStringLiteral("kolourpaint"),
                         i18n("KolourPaint"),
                         QStringLiteral(KOLOURPAINT_VERSION_STRING),
                         i18n("Paint Program by KDE"),
                         KAboutLicense::Custom,
                         QString(), // copyright statement - see license instead
                         QString(), // other text
                         QStringLiteral("http://www.kolourpaint.org/") // home page
    );

    // (this is _not_ the same as KAboutLicense::BSD)
    aboutData.setLicenseText(i18n(kpLicenseText));
    aboutData.setDesktopFileName(QStringLiteral("org.kde.kolourpaint"));

    // Please add yourself here if you feel you're missing.
    // SYNC: with AUTHORS

    aboutData.addAuthor(i18n("Clarence Dang"), i18n("Project Founder"), QStringLiteral("dang@kde.org"));

    aboutData.addAuthor(i18n("Thurston Dang"), i18n("Chief Investigator"), QStringLiteral("thurston_dang@users.sourceforge.net"));

    aboutData.addAuthor(i18n("Martin Koller"), i18n("Scanning Support, Alpha Support, Current Maintainer"), QStringLiteral("kollix@aon.at"));

    aboutData.addAuthor(i18n("Kristof Borrey"), i18n("Icons"), QStringLiteral("borrey@kde.org"));
    aboutData.addAuthor(i18n("Tasuku Suzuki"), i18n("InputMethod Support"), QStringLiteral("stasuku@gmail.com"));
    aboutData.addAuthor(i18n("Kazuki Ohta"), i18n("InputMethod Support"), QStringLiteral("mover@hct.zaq.ne.jp"));
    aboutData.addAuthor(i18n("Nuno Pinheiro"), i18n("Icons"), QStringLiteral("nf.pinheiro@gmail.com"));
    aboutData.addAuthor(i18n("Danny Allen"), i18n("Icons"), QStringLiteral("dannya40uk@yahoo.co.uk"));
    aboutData.addAuthor(i18n("Mike Gashler"), i18n("Image Effects"), QStringLiteral("gashlerm@yahoo.com"));

    aboutData.addAuthor(i18n("Laurent Montel"), i18n("KDE 4 Porting"), QStringLiteral("montel@kde.org"));
    aboutData.addAuthor(i18n("Christoph Feck"), i18n("KF 5 Porting"), QStringLiteral("cfeck@kde.org"));

    aboutData.addCredit(i18n("Thanks to the many others who have helped to make this program possible."));

    QCommandLineParser cmdLine;
    KAboutData::setApplicationData(aboutData);
    KCrash::initialize();
    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("kolourpaint"), QApplication::windowIcon()));
    cmdLine.addPositionalArgument(QStringLiteral("files"), i18n("Image files to open, optionally"), QStringLiteral("[files...]"));

    aboutData.setupCommandLine(&cmdLine);
    cmdLine.addOption(QCommandLineOption(QStringLiteral("mimetypes"), i18n("List all readable image MIME types")));
    cmdLine.addOption(QCommandLineOption(QStringLiteral("new"), i18n("Start with new image using given size"), i18n("[width]x[height]")));
    cmdLine.addOption(QCommandLineOption(QStringLiteral("no-ask-save"), i18n("Exit without asking to save changes")));
    cmdLine.process(app);
    aboutData.processCommandLine(&cmdLine);

    // produce a list of MimeTypes which kolourpaint can handle (can be used inside the .desktop file)
    if (cmdLine.isSet(QStringLiteral("mimetypes"))) {
        const QList<QByteArray> types = QImageReader::supportedMimeTypes();
        for (const QByteArray &type : types) {
            if (!type.isEmpty())
                printf("%s;", type.constData());
        }

        printf("\n");

        return 0;
    }

    if (app.isSessionRestored()) {
        // Creates a kpMainWindow using the default constructor and then
        // calls kpMainWindow::readProperties().
        kRestoreMainWindows<kpMainWindow>();
    } else {
        kpMainWindow *mainWindow;
        QStringList args = cmdLine.positionalArguments();

        if (args.count() >= 1) {
            for (int i = 0; i < args.count(); i++) {
                mainWindow = new kpMainWindow(QUrl::fromUserInput(args[i], QDir::currentPath(), QUrl::AssumeLocalFile));
                mainWindow->show();
            }
        } else {
            kpDocument *doc = nullptr;
            QString sizeStr = cmdLine.value(QStringLiteral("new"));

            if (!sizeStr.isEmpty()) {
                QStringList dimensions = sizeStr.split(QLatin1Char('x'));
                if (dimensions.count() == 2) {
                    unsigned int w = dimensions[0].toUInt();
                    unsigned int h = dimensions[1].toUInt();

                    if ((w > 0) && (h > 0))
                        doc = new kpDocument(w, h, nullptr);
                }
            }

            if (doc)
                mainWindow = new kpMainWindow(doc);
            else
                mainWindow = new kpMainWindow();

            mainWindow->show();
        }
    }

    kpMainWindow::setNoAskSave(cmdLine.isSet(QStringLiteral("no-ask-save")));

    return QApplication::exec();
}
