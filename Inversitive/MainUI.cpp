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

#include "MainUI.h"
#include "../Config.h"

#include <QtGui>
#include <QtCore>
#include <QUiLoader>
#include <exception>
#include <cmath>

#define MAX(a, b) (a > b? a : b)
#define MIN(a, b) (a < b? a : b)

namespace Inversitive {

MainUI::MainUI(QWidget *parent) : QWidget(), expanded(false) {
	ui_file = new QFile(":/form/inversitive.ui");
	if (!ui_file->open(QIODevice::ReadOnly))
		throw std::exception();

	setWindowIcon(QIcon(":/form/logo-svg"));
	setWindowTitle(QString("Inversitive UI"));

	loader = new QUiLoader(this);
	edit = new QTextEdit(this);
	save = new QPushButton("Save as .sns", this);
	edit->setReadOnly(true);
	loaded_widget = loader->load(ui_file);
	ui_file->close();

	layout = new QVBoxLayout(this);
	layout->addWidget(loaded_widget);
	layout->addWidget(edit);
	layout->addWidget(save);
	save->setVisible(false);

	QObject::connect(save, SIGNAL(clicked()), this, SLOT(SaveFile()));
	QObject::connect(loaded_widget->findChild<QPushButton*>("pushButton"), SIGNAL(clicked()), this, SLOT(Press()));

	animation = new QPropertyAnimation(this, "size");
	QObject::connect(animation, SIGNAL(finished()), this, SLOT(AnimationOver()));

	resize(layout->sizeHint());

	show();

}

void MainUI::Press() {
	loaded_widget->findChild<QPushButton*>("pushButton")->setEnabled(false);

	layout->setSizeConstraint(QLayout::SetNoConstraint);

	QLineEdit *linePol = loaded_widget->findChild<QLineEdit*>("lineEdit");

	QSpinBox *minimum = loaded_widget->findChild<QSpinBox*>("minimum");
	QSpinBox *maximum = loaded_widget->findChild<QSpinBox*>("maximum");

	int min = minimum->value();
	int max = maximum->value();

	animation->setStartValue(this->size());
	if (!expanded) {
		QString pol = linePol->text();
		QStringList str_coeffs = pol.split(' ');

		QString result(QString("# Generated by Inversitive %1 on %0\n# Coeffs: ").arg(QDate::currentDate().toString("dd MMMM")).arg(Config::VERSION));

		QList<double> coeffs;

		int i = str_coeffs.size()-1;
		foreach (QString s, str_coeffs) {
			bool ok = false;
			double t = s.toDouble(&ok);
			if (ok) {
				coeffs.insert(-1, t);
				result.append(QString("%0x^%1 ").arg(t).arg(i));
				i--;
			}
		}

		result.append("\n");

		for (int i = MIN(min, max); i <= MAX(min, max); i++) {
			double r = 0;
			int exp = 0;
			foreach (double d, coeffs) {
				r += d * pow(i, exp);
				exp++;
			}

			result.append(QString("%0:%1\n").arg(i).arg(r));
		}


		edit->setText(result);
		animation->setEndValue(QSize(this->size().width(), this->size().height() + 250));
	}

	else {
		edit->clear();
		animation->setEndValue(QSize(this->size().width(), this->size().height() - 250));
	}

	expanded = !expanded;

	animation->setDuration(1000);
	animation->setEasingCurve(QEasingCurve::OutBounce);
	animation->start();
}

void MainUI::SaveFile() {
	QString filename = QFileDialog::getSaveFileName(this, "Save as SNS", QDir::homePath(), "Sensitive Files (*.sns)");

	if (filename.isNull() || filename.isEmpty())
		return;

	QFile file(filename);

	if (!file.open(QFile::WriteOnly))
		return;

	QTextStream stream(&file);

	stream << edit->toPlainText();

	file.close();
}

void MainUI::AnimationOver() {
	QPushButton *b = loaded_widget->findChild<QPushButton*>("pushButton");
	if (expanded) {
		b->setText("Reset");
		save->setVisible(true);
	}

	else {
		save->setVisible(false);
		b->setText("Calculate");
	}

	b->setEnabled(true);
}

MainUI::~MainUI() {
	delete(ui_file);
}

}
