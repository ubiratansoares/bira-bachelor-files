
Name:		libHX18
%define lname	libHX
Version:	2.9
Release:	0
Group:		System/Libraries
URL:		http://libhx.sf.net/
Summary:	Library for commonly needed tasks in C
License:	LGPL2+
Source:		http://downloads.sf.net/libhx/libHX-%version.tar.bz2
BuildRoot:	%_tmppath/%name-%version-build
BuildRequires:	gcc-c++
# no, libxml2-devel is NOT required because nothing
# that requires it is going to be compiled.

%description
A library for:
- rbtree with key-value pair extension ("maps")
- deques (double-ended queues) (Stacks (LIFO) / Queues (FIFOs))
- platform-independent opendir-style directory access
- platform-independent dlopen-style shared library access
- auto-storage strings with direct access
- command line option (argv) parser
- shconfig-style config file parser
- platform-independent random number generator with transparent
  /dev/urandom support
- various string, memory and zvec ops

%package -n libHX-devel
Group:		Development/Libraries/C and C++
Summary:	Development files for libHX
Requires:	%name = %version

%description -n libHX-devel
A library for:
- rbtree with key-value pair extension
- deques (double-ended queues) (Stacks (LIFO) / Queues (FIFOs))
- platform independent opendir-style directory access
- platform independent dlopen-style shared library access
- auto-storage strings with direct access
- command line option (argv) parser
- shconfig-style config file parser
- platform independent random number generator with transparent
  /dev/urandom support
- various string, memory and zvec ops

%prep
%setup -n %lname-%version

%build
%configure
make %{?jobs:-j%jobs};

%install
b="%buildroot";
rm -Rf "$b";
mkdir "$b";
make install DESTDIR="$b" docdir="%_docdir/%lname";
rm -f "$b/%_libdir/%lname.la";
mkdir -p "$b/%_docdir/%lname";
install -pm0644 doc/* "$b/%_docdir/%lname/";

%post
%run_ldconfig

%postun
%run_ldconfig

%files
%defattr(-,root,root)
%_libdir/%{lname}*.so.*

%files -n libHX-devel
%defattr(-,root,root)
%_libdir/%{lname}*.so
%_libdir/pkgconfig/*
%_includedir/*
%docdir %_docdir/%lname
%doc %_docdir/%lname/*
