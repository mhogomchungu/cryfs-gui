#
# Spec file for package cryfs-gui
#
# Copyright © 2016 Francis Banyikwa <mhogomchungu@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

Name:           cryfs-gui
Version:        1.3.4
Release:        0
Summary:        Qt GUI front end to cryfs and encfs
License:        GPL-2.0+
Group:          Productivity/Security
Source:         %{name}-%{version}.tar.xz
Source100:      cryfs-gui-rpmlint
URL:            http://mhogomchungu.github.io/cryfs-gui

BuildRequires: cmake
BuildRequires: gcc-c++
BuildRequires: glibc-devel
BuildRequires: libgcrypt-devel
BuildRequires: libsecret-devel

%if 0%{?fedora}
BuildRequires: qt5-qtbase-devel
%else
#This package maybe named differently in your distribution.
#What is required here is a package or packages that provides development packages for Qt5Core,Qt5GUI and Qt5Network
BuildRequires: libqt5-qtbase-devel
%endif

%description
cryfs-gui is a Qt/C++ front end to encfs and cryfs.

%prep
%setup -q

%build
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DQT5=true -DNOKDESUPPORT=true -DNOSECRETSUPPORT=false -DCMAKE_BUILD_TYPE=RELEASE ..

%install
cd build
make DESTDIR=$RPM_BUILD_ROOT install

%clean
rm -rf %{buildroot}
rm -rf $RPM_BUILD_DIR/cryfs-gui

%files
%defattr(0755,root,root)
%{_bindir}/cryfs_gui
%{_datadir}/applications/cryfs-gui.desktop

%dir %{_datadir}/cryfs-gui/translations
%dir %{_datadir}/cryfs-gui

%defattr(0644,root,root)
%{_datadir}/icons/hicolor/48x48/apps/cryfs-gui.png
%{_datadir}/pixmaps/cryfs-gui.png
%{_datadir}/cryfs-gui/translations/en_US.qm
%{_datadir}/cryfs-gui/translations/fr_FR.qm
%defattr(0644,root,root)

%changelog
