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

options::options( QWidget * parent,std::function< void( const QString& ) >&& function ) :
	QDialog( parent ),m_ui( new Ui::options ),m_function( function )
{
	m_ui->setupUi( this ) ;

	this->setFixedSize( this->size() ) ;

	this->setWindowIcon( QIcon( ":/cryfs-gui.png" ) ) ;

	this->addAction( [ this ](){

		QAction * ac = new QAction( this ) ;

		connect( ac,SIGNAL( triggered() ),this,SLOT( defaultButton() ) ) ;

		return ac ;

	}() ) ;

	connect( m_ui->pbCancel,SIGNAL( clicked() ),this,SLOT( pbCancel() ) ) ;
	connect( m_ui->pbSet,SIGNAL( clicked() ),this,SLOT( pbSet() ) ) ;

	this->SetFocus() ;

	this->show() ;
}

void options::defaultButton()
{
	if( m_ui->pbCancel->hasFocus() ){

		this->pbCancel() ;
	}else{
		this->pbSet() ;
	}
}

void options::SetFocus()
{
	if( m_ui->lineEditKey->text().isEmpty() ){

		m_ui->lineEditKey->setFocus() ;
	}else{
		m_ui->pbSet->setFocus() ;
	}
}

void options::closeEvent( QCloseEvent * e )
{
	e->ignore() ;
	this->pbCancel() ;
}

void options::pbCancel()
{
	this->hide() ;
	this->deleteLater() ;
}

void options::pbSet()
{
	auto e = m_ui->lineEditKey->text() ;

	if( e.isEmpty() ){

		DialogMsg msg( this ) ;
		msg.ShowUIOK( tr( "ERROR" ),tr( "Option Field Is Empty." ) ) ;
	}else{
		m_function( e ) ;
		this->pbCancel() ;
	}
}

options::~options()
{	
}
