# -*- Makefile -*-

AUTOMAKE_OPTIONS = foreign subdir-objects

if GOT_TOOLS_FOR_DOC
doc_subdir       = doc
endif

if GOT_TOOLS_FOR_LOCALE
locale_subdir    = locale
endif

SUBDIRS          = etc user $(doc_subdir) $(locale_subdir)

.PHONY: tarball
tarball:
	rm -Rf /tmp/${PACKAGE_TARNAME}-${PACKAGE_VERSION};
	pushd ${top_srcdir} && git archive --prefix=${PACKAGE_TARNAME}-${PACKAGE_VERSION}/ HEAD | tar -C /tmp -x && popd;
	pushd /tmp/${PACKAGE_TARNAME}-${PACKAGE_VERSION} && ./autogen.sh && popd;
	tar -C /tmp -cjf ${PACKAGE_TARNAME}-${PACKAGE_VERSION}.tar.bz2 --owner=root --group=root ${PACKAGE_TARNAME}-${PACKAGE_VERSION}/;
	rm -Rf /tmp/${PACKAGE_TARNAME}-${PACKAGE_VERSION};
