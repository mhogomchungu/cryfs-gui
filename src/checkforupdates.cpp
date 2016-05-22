/*
 *
 *  Copyright (c) 2015
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

#include "checkforupdates.h"

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

#include <QWidget>
#include <QFile>

#include "utility.h"
#include "dialogmsg.h"

#include "version.h"

static void _show_cryfs_gui_version( QObject * obj,bool autocheck,QWidget * w,
				     QString l,const QStringList& e = QStringList() )
{
	DialogMsg msg( w ) ;

	if( l.isEmpty() ){

		msg.ShowUIOK( QObject::tr( "ERROR" ),QObject::tr( "Failed To Check For Update." ) ) ;
	}else{
		l.replace( "\n","" ) ;

		const auto& o = e.at( 0 ) ;
		const auto& n = e.at( 1 ) ;

		if( autocheck ){

			if( ( l != "Not Found" && l != THIS_VERSION ) || o != n ){

				l = "\n" + QObject::tr( "cryfs-gui Installed Version Is : %1.\nLatest Version Is : %2." ).arg( THIS_VERSION,l ) ;

				if( !e.isEmpty() ){

					l += "\n\n" + QObject::tr( "cryfs Installed Version Is : %1.\nLatest Version Is : %2." ).arg( o,n ) ;
				}

				msg.ShowUIOK( QObject::tr( "Update Available" ),l + "\n" ) ;
			}
		}else{
			if( l != "Not Found" ){

				l = "\n" + QObject::tr( "cryfs-gui Installed Version Is : %1.\nLatest Version Is : %2." ).arg( THIS_VERSION,l ) ;

				if( !e.isEmpty() ){

					l += "\n\n" + QObject::tr( "cryfs Installed Version Is : %1.\nLatest Version Is : %2." ).arg( o,n ) ;
				}

				msg.ShowUIOK( QObject::tr( "Version Info" ),l + "\n" ) ;
			}else{
				msg.ShowUIOK( QObject::tr( "ERROR" ),QObject::tr( "Failed To Check For Update." ) ) ;
			}
		}
	}

	obj->deleteLater() ;
}

#if QT_VERSION < QT_VERSION_CHECK( 5,0,0 )

void checkForUpdates::networkReply( QNetworkReply * p )
{
	_show_cryfs_gui_version( this,m_autocheck,m_widget,p->readAll() ) ;
}

#else

#include <QJsonDocument>

void checkForUpdates::networkReply( QNetworkReply * p )
{
	if( m_firstTime ){

		m_data = p->readAll() ;

		this->getUpdate( false ) ;
	}else{
		setenv( "CRYFS_NO_UPDATE_CHECK","TRUE",1 ) ;
		setenv( "CRYFS_FRONTEND","noninteractive",1 ) ;

		auto e = utility::Task::run( "cryfs" ).await().output().split( ' ' ) ;

		if( e.size() >= 2 ){

			QJsonParseError error ;

			auto r = QJsonDocument::fromJson( p->readAll(),&error ) ;

			if( error.error == QJsonParseError::NoError ){

				auto m = r.toVariant().toMap() ;

				if( !m.isEmpty() ){

					m = m[ "version_info" ].toMap() ;
				}

				if( !m.isEmpty() ){

					auto d = m[ "current" ].toString() ;

					if( !d.isEmpty() ){

						auto f = QString( e.at( 2 ) ).split( '\n' ).first() ;

						return _show_cryfs_gui_version( this,m_autocheck,m_widget,m_data,{ f,d } ) ;
					}
				}
			}
		}

		_show_cryfs_gui_version( this,m_autocheck,m_widget,m_data ) ;
	}
}

#endif

checkForUpdates::checkForUpdates( QWidget * widget,bool autocheck ) : m_widget( widget ),m_autocheck( autocheck )
{
	connect( &m_manager,SIGNAL( finished( QNetworkReply * ) ),this,SLOT( networkReply( QNetworkReply * ) ) ) ;

	this->getUpdate( true ) ;
}

void checkForUpdates::getUpdate( bool e )
{
	m_firstTime = e ;

	m_manager.get( [ & ](){

		auto e = [ & ](){

			if( m_firstTime ){

				QNetworkRequest e( QUrl( "https://raw.githubusercontent.com/mhogomchungu/cryfs-gui/master/version" ) ) ;
				e.setRawHeader( "Host","raw.githubusercontent.com" ) ;

				return e ;
			}else{
				QNetworkRequest e( QUrl( "https://www.cryfs.org/version_info.json" ) ) ;
				e.setRawHeader( "Host","www.cryfs.org" ) ;

				return e ;
			}
		}() ;

		e.setRawHeader( "User-Agent","Mozilla/5.0 (X11; Linux x86_64; rv:46.0) Gecko/20100101 Firefox/46.0" ) ;
		e.setRawHeader( "Accept-Encoding","text/plain" ) ;

		return e ;
	}() ) ;
}

static QString _optionPath()
{
	return utility::homePath() + "/.cryfs-gui/autoCheckUpdates.cryfs-gui" ;
}

void checkForUpdates::instance( QWidget * widget,const QString& e )
{
	Q_UNUSED( e ) ;

	if( utility::pathExists( _optionPath() ) ){

		new checkForUpdates( widget,true ) ;
	}
}

void checkForUpdates::instance( QWidget * widget )
{
	new checkForUpdates( widget,false ) ;
}

bool checkForUpdates::autoCheck()
{
	return utility::pathExists( _optionPath() ) ;
}

void checkForUpdates::autoCheck( bool e )
{
	QFile f( _optionPath() ) ;

	if( e ){

		f.open( QIODevice::WriteOnly | QIODevice::Truncate ) ;
	}else{
		f.remove() ;
	}
}

checkForUpdates::~checkForUpdates()
{
}
