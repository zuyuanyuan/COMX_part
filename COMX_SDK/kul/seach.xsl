<?xml version="1.0"?>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
		version="1.0"
		xmlns:kidl="http://www.kingmesh.com"
		xmlns:date="http://xml.apache.org/common"
                extension-element-prefixes="date">
  <xsl:output method="text" indent="yes"/>
  
  <xsl:template name="press_enter_key">
    <xsl:text>&#xa;</xsl:text>
  </xsl:template>

  <xsl:template match="/">
    <xsl:text>&lt;search_result&gt;</xsl:text>
    <xsl:call-template name="press_enter_key"/>
      <xsl:for-each select="//kidl:widget[@type='form']">
        <xsl:text>&lt;search_result_item&gt;</xsl:text>
        <xsl:value-of select="."/>
        <xsl:text>&lt;/search_result_item&gt;</xsl:text>
        <xsl:call-template name="press_enter_key"/>
      </xsl:for-each>
    <xsl:text>&lt;/search_result&gt;</xsl:text>
    <xsl:call-template name="press_enter_key"/>
  </xsl:template>

</xsl:stylesheet>