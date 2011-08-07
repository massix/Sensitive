/* Copyright (C) 2011 by Massimo Gengarelli <massimo.gengarelli@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "AboutDialog.h"

#include <QtGui>

#define LICENSE "Copyright (C) 2011 by Massimo Gengarelli &lt;massimo.gengarelli@gmail.com&gt;<br /> \
<br />\
Permission is hereby granted, free of charge, to any person obtaining a copy <br />\
of this software and associated documentation files (the \"Software\"), to deal <br />\
in the Software without restriction, including without limitation the rights <br />\
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell <br />\
copies of the Software, and to permit persons to whom the Software is <br />\
furnished to do so, subject to the following conditions: <br />\
<br />\
The above copyright notice and this permission notice shall be included in<br /> \
all copies or substantial portions of the Software.<br /> \
<br />\
<b>THE SOFTWARE IS PROVIDED \"AS IS\"</b>, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR <br />\
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, <br />\
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE<br /> \
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER <br />\
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, <br />\
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN<br /> \
THE SOFTWARE."

#define AUTHORS "Massimo Gengarelli &lt;massimo.gengarelli@gmail.com&gt;"

#define WEBSITE "http://git.casafamelica.info/sensitive.git"

Graphics::AboutDialog* Graphics::AboutDialog::instance = 0;

namespace Graphics {

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent) {
	main_widget = new QWidget(this);
	main_layout = new QVBoxLayout(main_widget);

	main_container = new QTabWidget();

	authors = new QTextEdit(AUTHORS);
	authors->setReadOnly(true);

	license = new QTextEdit(LICENSE);
	license->setReadOnly(true);

	website = new QTextEdit(WEBSITE);
	website->setReadOnly(true);

	logo = new QLabel();
	logo->setPixmap(QPixmap(":/bundle/logo-about"));

	description = new QLabel("<b>Sensitive Interpolation Software</b><br />"
			"Copyright(C) 2011 Massimo Gengarelli &lt;massimo.gengarelli@gmail.com&gt;.<br />"
			"See <i>License</i> tab for further details.");

	close_button = new QPushButton("Close Dialog");

	main_container->addTab(authors, "Authors");
	main_container->addTab(license, "License");
	main_container->addTab(website, "Website");

	main_layout->addWidget(logo);
	main_layout->addWidget(description);
	main_layout->addWidget(main_container);
	main_layout->addWidget(close_button);

	setModal(true);

	setWindowTitle(tr("About Sensitive"));

	update();

	setFixedSize(main_widget->sizeHint());

	QObject::connect(close_button, SIGNAL(clicked()), this, SLOT(accept()));
}

AboutDialog::~AboutDialog() {

}

AboutDialog* AboutDialog::GetInstance() {
	if (instance == 0)
		instance = new AboutDialog();

	return instance;
}

}
