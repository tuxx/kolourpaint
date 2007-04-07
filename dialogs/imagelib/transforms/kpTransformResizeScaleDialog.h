
/*
   Copyright (c) 2003-2007 Clarence Dang <dang@kde.org>
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef kpTransformResizeScaleDialog_H
#define kpTransformResizeScaleDialog_H


#include <qpixmap.h>

#include <kdialog.h>

#include <kpColor.h>
#include <kpCommandHistory.h>
#include <kpSelection.h>
#include <kpTransformResizeScaleCommand.h>


class QCheckBox;
class QGroupBox;
class QLabel;
class QRadioButton;
class QSize;
class QString;
class QToolButton;

class KComboBox;
class KDoubleNumInput;
class KHBox;
class KIntNumInput;

class kpDocument;
class kpMainWindow;
class kpViewManager;


class kpTransformResizeScaleDialog : public KDialog
{
Q_OBJECT

public:
    kpTransformResizeScaleDialog (kpMainWindow *mainWindow);
    virtual ~kpTransformResizeScaleDialog ();

    enum ActOn
    {
        Image, Selection
    };

private:
    static kpTransformResizeScaleCommand::Type s_lastType;
    static double s_lastPercentWidth, s_lastPercentHeight;

private:
    kpDocument *document () const;
    kpSelection *selection () const;

    void createActOnBox (QWidget *baseWidget);
    void createOperationGroupBox (QWidget *baseWidget);
    void createDimensionsGroupBox (QWidget *baseWidget);

    void widthFitHeightToAspectRatio ();
    void heightFitWidthToAspectRatio ();

private:
    bool resizeEnabled () const;
    bool scaleEnabled () const;
    bool smoothScaleEnabled () const;

public slots:
    void slotActOnChanged ();
    void slotTypeChanged ();

    void slotWidthChanged (int width);
    void slotHeightChanged (int height);

    void slotPercentWidthChanged (double percentWidth);
    void slotPercentHeightChanged (double percentHeight);

public:
    // (refers only to the state of the checkbox - user of dialog does
    //  not have to do extra calculations)
    bool keepAspectRatio () const;
public slots:
    void setKeepAspectRatio (bool on);

private:
    int originalWidth () const;
    int originalHeight () const;

public:
    int imageWidth () const;
    int imageHeight () const;
    bool actOnSelection () const;
    kpTransformResizeScaleCommand::Type type () const;

    bool isNoOp () const;

private slots:
    virtual void accept ();

private:
    kpMainWindow *m_mainWindow;

    KHBox *m_actOnBox;
    QLabel *m_actOnLabel;
    KComboBox *m_actOnCombo;

    QGroupBox *m_operationGroupBox;
    QToolButton *m_resizeButton,
                *m_scaleButton,
                *m_smoothScaleButton;
    QLabel *m_resizeLabel,
                 *m_scaleLabel,
                 *m_smoothScaleLabel;

    QGroupBox *m_dimensionsGroupBox;
    KIntNumInput *m_originalWidthInput, *m_originalHeightInput,
                 *m_newWidthInput, *m_newHeightInput;
    KDoubleNumInput *m_percentWidthInput, *m_percentHeightInput;
    QCheckBox *m_keepAspectRatioCheckBox;

    int m_ignoreKeepAspectRatio;
};


#endif  // kpTransformResizeScaleDialog_H
