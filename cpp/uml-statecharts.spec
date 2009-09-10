%define is_mandrake %(test -e /etc/mandrake-release && echo 1 || echo 0)
%define is_suse %(test -e /etc/SuSE-release && echo 1 || echo 0)
%define is_fedora %(test -e /etc/fedora-release && echo 1 || echo 0)

%define dist redhat
%define disttag rh

%if %is_mandrake
%define dist mandrake
%define disttag mdk
%endif
%if %is_suse
%define dist suse
%define disttag suse
%endif
%if %is_fedora
%define dist fedora
%define disttag fc
%endif

%define distver %(release="`rpm -q --queryformat='%{VERSION}' %{dist}-release 2> /dev/null | tr . : | sed s/://g`" ; if test $? != 0 ; then release="" ; fi ; echo "$release")
%define packer %(finger -lp `echo "$USER"` | head -n 1 | cut -d: -f 3)


Summary: UML statechart framework in C++.
Name: uml-statecharts
Version: 0.9.4
Release: 1.%{disttag}%{distver}
License: GNU Lesser General Public Licence (LGPL)
Group: Development/Libraries
URL: http://uml-statecharts.sourceforge.net/
Source0: %{name}-%{version}.zip
Patch0: %{name}-%{version}.patch
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
Packager: %packer

%description 
The framework implements UML statecharts in C++ and Java. Next to the
standard elements of FSMs, hierarchical and concurrent states are as
well supported as pseudostates, segmented transitions, timed events
and an eventqueue using a thread pool.

%package devel
Summary: uml-statecharts development files
Group: Development/Libraries
Provides: uml-statecharts-devel

%description devel
This package contains necessary header files for uml-statechart development.

%prep
%setup -q
%patch0 -p1

%build
cd cpp/src/statechart
make

%install
rm -rf $RPM_BUILD_ROOT
cd cpp/src/statechart
prefix=$RPM_BUILD_ROOT/usr make install

%post
cat << EOF | ed %{_libdir}/pkgconfig/statechart.pc
/prefix=/
c
prefix=%{_prefix}
.
w
q
EOF

%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%{_libdir}

%files devel
%{_includedir}/*
%{_libdir}/pkgconfig/statechart.pc

%changelog
* Sun Jun 21 2009 Glen Cornell <glencoindetroit@aim.com> - 
- Initial build.
