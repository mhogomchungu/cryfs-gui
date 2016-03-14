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

#include "monitor_mountinfo.h"

#include <QString>
#include <QStringList>
#include <QDebug>

#include "utility.h"
#include "task.h"
#include "cryfstask.h"

#include <QCoreApplication>

monitor_mountinfo::monitor_mountinfo( QObject * parent,bool e,std::function< void() >&& f ) :
	QThread( parent ),m_stop( std::move( f ) ),m_announceEvents( e )
{
	m_babu = parent ;
	m_baba = this ;
	m_main = this ;
}

monitor_mountinfo::~monitor_mountinfo()
{
}

std::function< void() > monitor_mountinfo::stop()
{
	return [ this ](){

		if( m_running ){

			m_mtoto->terminate() ;
		}else{
			this->threadStopped() ;
		}
	} ;
}

void monitor_mountinfo::threadStopped()
{
	m_running = false ;
	m_stop() ;
}

void monitor_mountinfo::failedToStart()
{
	qDebug() << "failed to monitor /proc/self/mountinfo" ;
	m_running = false ;
}

void monitor_mountinfo::announceEvents( bool s )
{
	m_announceEvents = s ;
}

void monitor_mountinfo::run()
{
	m_mtoto = this ;

	connect( m_mtoto,SIGNAL( finished() ),m_main,SLOT( threadStopped() ) ) ;
	connect( m_mtoto,SIGNAL( finished() ),m_mtoto,SLOT( deleteLater() ) ) ;

	utility::monitor_mountinfo monitor ;

	if( monitor ){

		m_running = true ;
	}else{
		return this->failedToStart() ;
	}

	while( monitor.gotEvent() ){

		if( m_announceEvents ){

			emit gotEvent() ;
		}
	}
}
