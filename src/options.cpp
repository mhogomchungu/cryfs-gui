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

#include "options.h"
#include "ui_options.h"
#include "dialogmsg.h"

#include "utility.h"

#include <QFileDialog>

options::options( QWidget * parent,
		  std::function< void( const QStringList& ) >&& e ) :
	QDialog( parent ),
	m_ui( new Ui::options ),
	m_setOptions( std::move( e ) )
{
	m_ui->setupUi( this ) ;

	this->setFixedSize( this->size() ) ;

        m_ui->pushButton->setIcon( QIcon( ":/file.png" ) ) ;

	this->setWindowIcon( QIcon( ":/cryfs-gui.png" ) ) ;

        connect( m_ui->pushButton,SIGNAL( clicked() ),this,SLOT( pushButton() ) ) ;

	connect( m_ui->pbOK,SIGNAL( clicked() ),this,SLOT( pbOK() ) ) ;

	m_ui->lineEditIdleTime->setFocus() ;

	this->show() ;
}

void options::pushButton()
{
        auto e = QFileDialog::getOpenFileName( this,tr( "Select Cryfs Configuration File" ),
                                               utility::homePath() ) ;

        if( !e.isEmpty() ){

		m_ui->lineConfigFilePath->setText( e ) ;
        }
}

void options::closeEvent( QCloseEvent * e )
{
	e->ignore() ;
	this->pbOK() ;
}

void options::pbOK()
{
	auto e = m_ui->lineEditIdleTime->text() ;

	bool ok = true ;

	if( !e.isEmpty() ){

		e.toInt( &ok ) ;
	}

	if( !ok ){

		DialogMsg msg( this ) ;
		msg.ShowUIOK( tr( "ERROR" ),tr( "Idle Time Field Requires Digits Only If Not Empty." ) ) ;
	}else{
		m_setOptions( { e,m_ui->lineConfigFilePath->text() } ) ;

		this->hide() ;
		this->deleteLater() ;
	}
}

options::~options()
{
	delete m_ui ;
}
