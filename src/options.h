/*
 *
 *  Copyright (c) 2012-2015
 *  name : Francis Banyikwa
 *  email: mhogomchungu@gmail.com
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <QWidget>

#include <QObject>
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QAction>
#include <QCloseEvent>

#include <functional>

namespace Ui {
class options;
}

class options : public QDialog
{
	Q_OBJECT
public:
        static void instance( QWidget * parent,bool n,const QString& e,
                              std::function< void( const QString& ) >&& function )
	{
                new options( parent,n,e,std::move( function ) ) ;
	}

        options( QWidget * parent,bool,const QString&,std::function< void( const QString& ) >&& ) ;
	~options();
private slots:
        void pushButton( void ) ;
	void defaultButton( void ) ;
	void pbSet( void ) ;
	void pbCancel( void ) ;
private:
	void closeEvent( QCloseEvent * ) ;
	void SetFocus( void );
	Ui::options * m_ui ;
	std::function< void( const QString& ) > m_function ;
};

#endif // OPTIONS_H
