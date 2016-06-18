<?xml version="1.0"?>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
		version="1.0"
		xmlns:kidl="http://www.kingmesh.com"
		xmlns:date="http://xml.apache.org/common"
                extension-element-prefixes="date">
  <xsl:output method="text" indent="yes"/>
  
  <xsl:template match="/">
    <xsl:for-each select="kidl:kul_pkg">
      <xsl:call-template name="header_comment_block">
        <xsl:with-param name="kul_pkg" select="."/>
      </xsl:call-template>

      <xsl:call-template name="copyright_comment_block">
        <xsl:with-param name="kul_pkg" select="."/>
      </xsl:call-template>
      
      <xsl:call-template name="begin_hxx_block">
        <xsl:with-param name="kul_pkg" select="."/>
      </xsl:call-template>
      
      <xsl:call-template name="include_header_segment">
        <xsl:with-param name="kul_pkg" select="."/>
      </xsl:call-template>
      
      <xsl:call-template name="press_enter_key"/>
      <xsl:call-template name="namespace_block_begin">
	   <xsl:with-param name="kul_pkg" select="."/>
      </xsl:call-template>
      
      <xsl:for-each select="kidl:pkgid">
        <xsl:call-template name="press_enter_key"/>
        <xsl:call-template name="pkgid_segment">
	     <xsl:with-param name="pkgid" select="."/>
        </xsl:call-template>    
      </xsl:for-each>
      
      <xsl:for-each select="kidl:formid">
        <xsl:call-template name="press_enter_key"/>
        <xsl:call-template name="formid_segment">
	     <xsl:with-param name="formid" select="."/>
        </xsl:call-template>    
      </xsl:for-each>
      
      <xsl:call-template name="press_enter_key"/>
      <xsl:call-template name="namespace_block_end">
	   <xsl:with-param name="kul_pkg" select="."/>
      </xsl:call-template>
      
      <xsl:call-template name="end_hxx_block">
        <xsl:with-param name="kul_pkg" select="."/>
      </xsl:call-template>
    </xsl:for-each>
  </xsl:template>
  
  <xsl:template name="header_comment_block">
    <xsl:param name="kul_pkg"/>
    
    <xsl:text>/*HEAD </xsl:text>
    <xsl:value-of select="kidl:to_lower(kidl:output_filename())"/>
    <xsl:text> HXX </xsl:text>
    <xsl:variable name="category" select="$kul_pkg/@name"/>
    <xsl:value-of select="kidl:to_upper($category)"/>
    <xsl:text>*/</xsl:text>
    <xsl:call-template name="press_enter_key"/>
  </xsl:template>
  
  <xsl:template name="press_enter_key">
    <xsl:text>&#xa;</xsl:text>
  </xsl:template>
  
  <xsl:template name="copyright_comment_block">
    <xsl:param name="kul_pkg"/>
    <xsl:call-template name="press_enter_key"/>
    <xsl:text>/*==============================================================================</xsl:text>
    <xsl:call-template name="press_enter_key"/>
    <xsl:call-template name="press_enter_key"/>
    <xsl:text>                      Copyright </xsl:text>
    <xsl:value-of select="kidl:get_year()"/>
    <xsl:text> Kingmesh Corp.</xsl:text>
    <xsl:call-template name="press_enter_key"/>
    <xsl:text>                           All rights reserved</xsl:text>
    <xsl:call-template name="press_enter_key"/>
    <xsl:call-template name="press_enter_key"/>
    <xsl:text>==============================================================================*/</xsl:text>
    <xsl:call-template name="press_enter_key"/>
    <xsl:call-template name="press_enter_key"/>
  </xsl:template>
  
  <xsl:template name="begin_hxx_block">
    <xsl:param name="kul_pkg"/>

    <xsl:text>#ifndef _KMAS_COMX_KULID_</xsl:text>
    <xsl:variable name="clsid" select="$kul_pkg/kidl:pkgid/kidl:value"/>
    <xsl:value-of select="kidl:simplify_muid(string($clsid))"/>
    <xsl:call-template name="press_enter_key"/>

    <xsl:text>#define _KMAS_COMX_KULID_</xsl:text>
    <xsl:value-of select="kidl:simplify_muid(string($clsid))"/>
    <xsl:call-template name="press_enter_key"/>
    <xsl:call-template name="press_enter_key"/>
    
  </xsl:template>
  
  <xsl:template name="end_hxx_block">
    <xsl:call-template name="press_enter_key"/>
    <xsl:text>#endif</xsl:text>
    <xsl:call-template name="press_enter_key"/>
  </xsl:template>
  
  <xsl:template name="include_header_segment">
    <xsl:param name="kul_pkg"/>
      <xsl:text>#include &lt;base/root.hxx></xsl:text>
      <xsl:call-template name="press_enter_key"/>
      <xsl:text>using namespace KMAS::Die_maker::comx;</xsl:text>
      <xsl:call-template name="press_enter_key"/>
  </xsl:template>
  
    <xsl:template name="namespace_block_begin">
    <xsl:param name="kul_pkg"/>

    <xsl:variable name="category" select="$kul_pkg/@name"/>
    <xsl:text>namespace comx{</xsl:text>
    <xsl:call-template name="press_enter_key"/>
    <xsl:if test="kidl:to_lower($category) != 'comx_sdk'">
      <xsl:text>namespace </xsl:text>
      <xsl:value-of select="kidl:to_lower($category)"/>
      <xsl:text>{</xsl:text>
      <xsl:call-template name="press_enter_key"/>
    </xsl:if>
    <xsl:text>namespace kul{</xsl:text>
    <xsl:call-template name="press_enter_key"/>

  </xsl:template>

  <xsl:template name="namespace_block_end">
    <xsl:param name="kul_pkg"/>

    <xsl:text>}//namespace kul</xsl:text>
    <xsl:call-template name="press_enter_key"/>
    <xsl:variable name="category" select="$kul_pkg/@name"/>
    <xsl:if test="kidl:to_lower($category) != 'comx_sdk'">
      <xsl:text>}//namespace </xsl:text>
      <xsl:value-of select="kidl:to_lower($category)"/>
      <xsl:call-template name="press_enter_key"/>
    </xsl:if>
    <xsl:text>}//namespace comx</xsl:text>
    <xsl:call-template name="press_enter_key"/>
    
  </xsl:template>
  
  <xsl:template name="pkgid_segment">
    <xsl:param name="pkgid"/>

    <xsl:text>// {</xsl:text>
    <xsl:variable name="clsid" select="$pkgid/kidl:value"/>
    <xsl:value-of select="$clsid"/>
    <xsl:text>}</xsl:text>
    <xsl:call-template name="press_enter_key"/>
    <xsl:text>const MTYPEID PKGID_I</xsl:text>
    <xsl:variable name="pkgid_name" select="$pkgid/@name"/>
    <xsl:value-of select="kidl:to_name($pkgid_name)"/>
    <xsl:text> = </xsl:text>
    <xsl:call-template name="press_enter_key"/>
    <xsl:value-of select="kidl:const_muid($clsid)"/>
    <xsl:call-template name="press_enter_key"/>
    
  </xsl:template>
  
  <xsl:template name="formid_segment">
    <xsl:param name="formid"/>

    <xsl:text>// {</xsl:text>
    <xsl:variable name="clsid" select="$formid/kidl:value"/>
    <xsl:value-of select="$clsid"/>
    <xsl:text>}</xsl:text>
    <xsl:call-template name="press_enter_key"/>
    <xsl:text>const MTYPEID FORMID_I</xsl:text>
    <xsl:variable name="formid_name" select="$formid/@name"/>
    <xsl:value-of select="kidl:to_name($formid_name)"/>
    <xsl:text> = </xsl:text>
    <xsl:call-template name="press_enter_key"/>
    <xsl:value-of select="kidl:const_muid($clsid)"/>
    <xsl:call-template name="press_enter_key"/>
    
  </xsl:template>
</xsl:stylesheet>