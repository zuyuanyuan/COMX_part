@echo off
@XalanTransform %1 kidl.fo.xsl y:\tmp.fo
@fop y:\tmp.fo %2
@del y:\tmp.fo
