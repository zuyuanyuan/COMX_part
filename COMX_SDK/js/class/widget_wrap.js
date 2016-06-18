/*
 * Scintilla Widget JS Class Wrap.
 */

function TScintillaWnd(hdr)
{
	this.hdr = hdr;
}

function TScintillaWnd_Paste()
{
	widget.scintilla.Paste(this.hdr);
}

function TScintillaWnd_SetFirstVisibleLine(line_no)
{
	widget.scintilla.SetFirstVisibleLine(this.hdr, line_no);
}

function TScintillaWnd_GetFirstVisibleLine()
{
	return widget.scintilla.GetFirstVisibleLine(this.hdr);
}

function TScintillaWnd_Copy()
{
	return widget.scintilla.Copy(this.hdr);
}

function TScintillaWnd_Redo()
{
	return widget.scintilla.Redo(this.hdr);
}

function TScintillaWnd_Undo()
{
	return widget.scintilla.Undo(this.hdr);
}

function TScintillaWnd_Cut()
{
	return widget.scintilla.Cut(this.hdr);
}

function TScintillaWnd_Clear()
{
	return widget.scintilla.Clear(this.hdr);
}

function TScintillaWnd_GetSize()
{
	return widget.scintilla.GetSize(this.hdr);
}

function TScintillaWnd_GetLineCount()
{
	return widget.scintilla.GetLineCount(this.hdr);
}

function TScintillaWnd_IsDirty()
{
	return widget.scintilla.IsDirty(this.hdr);
}

function TScintillaWnd_ClearDirtyFlag()
{	
	return widget.scintilla.ClearDirtyFlag(this.hdr);
}

function TScintillaWnd_GetPosition()
{
	return widget.scintilla.GetPosition(this.hdr);
}

function TScintillaWnd_SetPosition(pos)
{
	return widget.scintilla.SetPosition(this.hdr, pos);
}

TScintillaWnd.prototype.Paste               = TScintillaWnd_Paste;
TScintillaWnd.prototype.Copy                = TScintillaWnd_Copy;
TScintillaWnd.prototype.GetFirstVisibleLine = TScintillaWnd_GetFirstVisibleLine;
TScintillaWnd.prototype.SetFirstVisibleLine = TScintillaWnd_SetFirstVisibleLine;
TScintillaWnd.prototype.Undo                = TScintillaWnd_Undo;
TScintillaWnd.prototype.Redo                = TScintillaWnd_Redo;
TScintillaWnd.prototype.Cut                 = TScintillaWnd_Cut;
TScintillaWnd.prototype.Clear               = TScintillaWnd_Clear;
TScintillaWnd.prototype.GetSize             = TScintillaWnd_GetSize;
TScintillaWnd.prototype.GetLineCount        = TScintillaWnd_GetLineCount;
TScintillaWnd.prototype.IsDirty             = TScintillaWnd_IsDirty;
TScintillaWnd.prototype.ClearDirtyFlag      = TScintillaWnd_ClearDirtyFlag;
TScintillaWnd.prototype.GetPosition         = TScintillaWnd_GetPosition;
TScintillaWnd.prototype.SetPosition         = TScintillaWnd_SetPosition;

/*
 * Bitmap Widget JS Class Wrap.
 */

function TBitmapWnd(hdr)
{
	this.hdr = hdr;
	widget.bitmap.InitializeCanvas(hdr);
}

function TBitmapWnd_SaveAs(fpath)
{
	return widget.bitmap.SaveAs(this.hdr, fpath);
}

function TBitmapWnd_SetDisplayMode(mode)
{
	return widget.bitmap.SetDisplayMode(this.hdr, mode);
}

function TBitmapWnd_SetBackColor(red, green, blue)
{
	return widget.bitmap.SetBackColor(this.hdr, red, green, blue);
}

function TBitmapWnd_SetTransparentColor(red, green, blue)
{
	return widget.bitmap.SetTransparentColor(this.hdr, red, green, blue);
}

function TBitmapWnd_EnableTransparentMode(flag)
{
	return widget.bitmap.EnableTransparentMode(this.hdr, flag);
}

function TBitmapWnd_AddPoint(x, y)
{
	return widget.bitmap.AddPoint(this.hdr, x, y);
}

function TBitmapWnd_ClearPoints()
{
	return widget.bitmap.ClearPoints(this.hdr);
}

TBitmapWnd.prototype.SaveAs                = TBitmapWnd_SaveAs;
TBitmapWnd.prototype.SetDisplayMode        = TBitmapWnd_SetDisplayMode;
TBitmapWnd.prototype.SetBackColor          = TBitmapWnd_SetBackColor;
TBitmapWnd.prototype.SetTransparentColor   = TBitmapWnd_SetTransparentColor;
TBitmapWnd.prototype.EnableTransparentMode = TBitmapWnd_EnableTransparentMode;
TBitmapWnd.prototype.AddPoint              = TBitmapWnd_AddPoint; 
TBitmapWnd.prototype.ClearPoints           = TBitmapWnd_ClearPoints;

/*
 * Toolbar Widget JS Class Wrap.
 */

function TToolbar(toolbarid)
{
	this.toolbarid = toolbarid;
}

function TToolbar_Activate(index)
{
	comx.toolbar.Activate(this.toolbarid, index);
}

function TToolbar_Check(index, flag)
{
	comx.toolbar.Check(this.toolbarid, index, flag);
}

function TToolbar_IsCheck(index)
{
	return comx.toolbar.IsCheck(this.toolbarid, index);
}

function TToolbar_Enable(index, flag)
{
	comx.toolbar.Enable(this.toolbarid, index, flag);
}

function TToolbar_IsEnable(index)
{
	return comx.toolbar.IsEnable(this.toolbarid, index);
}

function TToolbar_Relayout()
{
	comx.toolbar.Relayout();
}

function TToolbar_Show()
{
	comx.toolbar.Show(this.toolbarid);
}

function TToolbar_Hide()
{
	comx.toolbar.Hide(this.toolbarid);
}

TToolbar.prototype.Activate = TToolbar_Activate;
TToolbar.prototype.Check = TToolbar_Check;
TToolbar.prototype.IsCheck = TToolbar_IsCheck;
TToolbar.prototype.Enable = TToolbar_Enable;
TToolbar.prototype.IsEnable = TToolbar_IsEnable;
TToolbar.prototype.Relayout = TToolbar_Relayout;
TToolbar.prototype.Hide = TToolbar_Hide;
TToolbar.prototype.Show = TToolbar_Show;

/*
 * Xml Document JS Class Wrap.
 */

function TXmlDocument(encode)
{
	var encoding = "";
	if(typeof(encode) != "undefined")
		encoding = encode;
	else
		encoding = "utf-8";

	this.hdr = comx.xml.CreateHandler(encoding);
}

function TXmlDocument_Dispose()
{
	comx.xml.DestroyHandler(this.hdr);
}

function TXmlDocument_Load(file_path)
{
	comx.xml.Load(this.hdr, file_path);
}

function TXmlDocument_Save(file_path)
{	
	comx.xml.Save(this.hdr, file_path);
}

function TXmlDocument_Beautify(file_path)
{
	comx.shell.beautify_xml(file_path);
}

function TXmlDocument_Search(key)
{
	return parseInt(comx.xml.Search(this.hdr, key));
}

function TXmlDocument_ClearResults()
{
	return comx.xml.ClearRecords();
}

function TXmlDocument_GetRecordsNumber()
{
	return parseInt(comx.xml.GetRecordsNumber());
}

function TXmlDocument_GetRecord(index)
{
	return comx.xml.GetRecord(index);
}

function TXmlDocument_ResetPos()
{
	return comx.xml.ResetPos(this.hdr);
}

function TXmlDocument_SavePos(name)
{
	return comx.xml.SavePos(this.hdr, name);
}

function TXmlDocument_RestorePos(name)
{
	return comx.xml.RestorePos(this.hdr, name);
}

function TXmlDocument_FindElem(name)
{
	return comx.xml.FindElem(this.hdr, name);
}

function TXmlDocument_IntoElem()
{
	return comx.xml.IntoElem(this.hdr);
}

function TXmlDocument_OutOfElem()
{
	return comx.xml.OutOfElem(this.hdr);
}

function TXmlDocument_GetData()
{
	return comx.xml.GetData(this.hdr);
}

function TXmlDocument_GetAttrib()
{
	return comx.xml.GetAttrib(this.hdr);
}

function TXmlDocument_SetData(data)
{
	return comx.xml.SetData(this.hdr, data);
}

function TXmlDocument_SetAttrib(attr)
{
	return comx.xml.SetAttrib(this.hdr, attr);
}

function TXmlDocument_AddElem(elem_name)
{
	return comx.xml.AddElem(this.hdr, elem_name);
}

function TXmlDocument_InserElem(elem_name)
{
	return comx.xml.InserElem(this.hdr, elem_name);
}

function TXmlDocument_IsWellFormat()
{
	return Boolean(comx.xml.IsWellFormat(this.hdr));
}

function TXmlDocument_RemoveElem()
{
	return comx.xml.RemoveElem(this.hdr);
}

TXmlDocument.prototype.Dispose = TXmlDocument_Dispose;
TXmlDocument.prototype.Load = TXmlDocument_Load;
TXmlDocument.prototype.Save = TXmlDocument_Save;
TXmlDocument.prototype.Beautify = TXmlDocument_Beautify;
TXmlDocument.prototype.Search = TXmlDocument_Search;
TXmlDocument.prototype.ClearRecords = TXmlDocument_ClearResults;
TXmlDocument.prototype.GetRecordsNumber = TXmlDocument_GetRecordsNumber;
TXmlDocument.prototype.GetRecord = TXmlDocument_GetRecord;
TXmlDocument.prototype.ResetPos = TXmlDocument_ResetPos;
TXmlDocument.prototype.SavePos = TXmlDocument_SavePos;
TXmlDocument.prototype.RestorePos = TXmlDocument_RestorePos;
TXmlDocument.prototype.FindElem = TXmlDocument_FindElem;
TXmlDocument.prototype.IntoElem = TXmlDocument_IntoElem;
TXmlDocument.prototype.OutOfElem = TXmlDocument_OutOfElem;
TXmlDocument.prototype.GetData = TXmlDocument_GetData;
TXmlDocument.prototype.GetAttrib = TXmlDocument_GetAttrib;
TXmlDocument.prototype.SetData = TXmlDocument_SetData;
TXmlDocument.prototype.SetAttrib = TXmlDocument_SetAttrib;
TXmlDocument.prototype.AddElem = TXmlDocument_AddElem;
TXmlDocument.prototype.InserElem = TXmlDocument_InserElem;
TXmlDocument.prototype.IsWellFormat = TXmlDocument_IsWellFormat;
TXmlDocument.prototype.RemoveElem = TXmlDocument_RemoveElem;

/*
 * glcanvas Widget JS Class Wrap.
 */

function TOpenGLWnd(window_hdr, db_hdr)
{
	this.window_hdr = window_hdr;
	this.db_hdr = db_hdr;
}

function TOpenGLWnd_CreateKey(code, val) //code should be >=100.
{
	return gl.database.CreateKey(code, val);
}

function TOpenGLWnd_AppendRecord(key)
{
	gl.database.AppendRecord(this.db_hdr, key);
}

function TOpenGLWnd_DeleteRecord(key)
{
	gl.database.DeleteRecord(this.db_hdr, key);
}

function TOpenGLWnd_DeleteAllRecords()
{
    gl.database.DeleteAllRecords(this.db_hdr);
}

function TOpenGLWnd_IsExistRecord(key)
{
	return gl.database.IsExistRecord(this.db_hdr, key);
}

function TOpenGLWnd_GetFirstRecord()
{
	return gl.database.GetFirstRecord(this.db_hdr);
}

function TOpenGLWnd_GetNextRecord() // return value format "tag_val, is_end", such as: "32124322,false". 
{
	return gl.database.GetNextRecord(this.db_hdr);
}

function TOpenGLWnd_GetRecord(key)
{
	return gl.database.GetRecord(this.db_hdr, key);
}

function TOpenGLWnd_GetRecordCount()
{
	return gl.database.GetRecordCount(this.db_hdr);
}

function TOpenGLWnd_CleanDrawList()
{
	gl.database.CleanDrawList(this.db_hdr);
}

function TOpenGLWnd_EnableLight(key)
{
	gl.database.EnableLight(this.db_hdr, key);
}

function TOpenGLWnd_ClearList(list_id)
{
	return gl.database.ClearList(this.db_hdr, list_id);
}

function TOpenGLWnd_BeginList(list_id)
{
	return gl.database.BeginList(this.db_hdr, list_id);
}

function TOpenGLWnd_EndList()
{
	return gl.database.EndList(this.db_hdr);
}

// 0 - GL_POINTS, 1 - GL_LINES, 4 - GL_TRIANGLES
function TOpenGLWnd_BeginPrimitive(mode)
{
	return gl.database.BeginPrimitive(this.db_hdr, mode);
}

function TOpenGLWnd_EndPrimitive()
{
	return gl.database.EndPrimitive(this.db_hdr);
}

function TOpenGLWnd_InputVertex(x, y, z)
{
	gl.database.InputVertex(this.db_hdr, x, y, z);
}

function TOpenGLWnd_InputNormal(nx, ny, nz)
{
	gl.database.InputNormal(this.db_hdr, nx, ny, nz);
}

function TOpenGLWnd_RefreshGlCanvas()
{
	return gl.database.RefreshGlCanvas(this.window_hdr);
}

function TOpenGLWnd_GetDisplayListIDOfRecord_(db_rec)
{
	return gl.database.GetDisplayListIDOfRecord(this.db_hdr, db_rec);
}

TOpenGLWnd.prototype.CreateKey = TOpenGLWnd_CreateKey;
TOpenGLWnd.prototype.AppendRecord = TOpenGLWnd_AppendRecord;
TOpenGLWnd.prototype.DeleteRecord = TOpenGLWnd_DeleteRecord;
TOpenGLWnd.prototype.DeleteAllRecords = TOpenGLWnd_DeleteAllRecords;
TOpenGLWnd.prototype.IsExistRecord = TOpenGLWnd_IsExistRecord;
TOpenGLWnd.prototype.GetFirstRecord = TOpenGLWnd_GetFirstRecord;
TOpenGLWnd.prototype.GetNextRecord = TOpenGLWnd_GetNextRecord;
TOpenGLWnd.prototype.GetRecord = TOpenGLWnd_GetRecord;
TOpenGLWnd.prototype.GetRecordCount = TOpenGLWnd_GetRecordCount;
TOpenGLWnd.prototype.CleanDrawList = TOpenGLWnd_CleanDrawList;
TOpenGLWnd.prototype.EnableLight = TOpenGLWnd_EnableLight;
TOpenGLWnd.prototype.ClearList = TOpenGLWnd_ClearList;
TOpenGLWnd.prototype.BeginList = TOpenGLWnd_BeginList;
TOpenGLWnd.prototype.EndList = TOpenGLWnd_EndList;
TOpenGLWnd.prototype.BeginPrimitive = TOpenGLWnd_BeginPrimitive;
TOpenGLWnd.prototype.EndPrimitive = TOpenGLWnd_EndPrimitive;
TOpenGLWnd.prototype.InputVertex = TOpenGLWnd_InputVertex;
TOpenGLWnd.prototype.InputNormal = TOpenGLWnd_InputNormal;
TOpenGLWnd.prototype.RefreshGlCanvas = TOpenGLWnd_RefreshGlCanvas;
TOpenGLWnd.prototype.GetDisplayListOfRecord = TOpenGLWnd_GetDisplayListIDOfRecord_;

function TOpenGLRecord(db_hdr, db_rec)
{
	this.db_rec = db_rec;
	this.db_hdr = db_hdr;
}

function TOpenGLWnd_GetKeyOfGlRecord()
{
	return gl.database.GetKeyOfGlRecord(this.db_hdr, this.db_rec);
}

function TOpenGLWnd_GetDisplayListIDOfRecord()
{
	return gl.database.GetDisplayListIDOfRecord(this.db_hdr, this.db_rec);
}

function TOpenGLWnd_GetRenderModeOfRecord()
{
	return gl.database.GetRenderModeOfRecord(this.db_hdr, this.db_rec);
}

function TOpenGLWnd_SetRenderModeOfRecord(mode) // 0 - common; 1 - highlight; 2 - mark.
{
	return gl.database.SetRenderModeOfRecord(this.db_hdr, this.db_rec, mode);
}

function TOpenGLWnd_GetColorOfRecord() // "r, g, b, a", such as : "255, 255, 0, 0".
{
	return gl.database.GetColorOfRecord(this.db_hdr, this.db_rec);
}

function TOpenGLWnd_SetColorOfRecord(clr)
{
	return gl.database.SetColorOfRecord(this.db_hdr, this.db_rec, clr);
}

function TOpenGLWnd_GetHightLightColorOfRecord() // "r, g, b, a", such as : "255, 255, 0, 0".
{
	return gl.database.GetHightLightColorOfRecord(this.db_hdr, this.db_rec);
}

function TOpenGLWnd_SetHightLightColorOfRecord(clr)
{
	return gl.database.SetHightLightColorOfRecord(this.db_hdr, this.db_rec, clr);
}

function TOpenGLWnd_GetMarkColorOfRecord() // "r, g, b, a", such as : "255, 255, 0, 0".
{
	return gl.database.GetMarkColorOfRecord(this.db_hdr, this.db_rec);
}

function TOpenGLWnd_SetMarkColorOfRecord(clr)
{
	return gl.database.SetMarkColorOfRecord(this.db_hdr, this.db_rec, clr);
}

function TOpenGLWnd_GetPolygonModeOfRecord() // 0 - Wire, 1 - Face, 2 - Vertex.
{
	return gl.database.GetPolygonModeOfRecord(this.db_hdr, this.db_rec);
}

function TOpenGLWnd_SetPolygonModeOfRecord(mode)
{
	return gl.database.SetPolygonModeOfRecord(this.db_hdr, this.db_rec, mode);
}

function TOpenGLWnd_GetLineWidthOfRecord()
{
	return gl.database.GetLineWidthOfRecord(this.db_hdr, this.db_rec);
}

function TOpenGLWnd_SetLineWidthOfRecord(line_width)
{
	return gl.database.SetLineWidthOfRecord(this.db_hdr, this.db_rec, line_width);
}

function TOpenGLWnd_GetPointSizeOfRecord()
{
	return gl.database.GetPointSizeOfRecord(this.db_hdr, this.db_rec);
}

function TOpenGLWnd_SetPointSizeOfRecord(point_size)
{
	return gl.database.SetPointSizeOfRecord(this.db_hdr, this.db_rec, point_size);
}

function TOpenGLWnd_IsLightOfRecord()
{
	return gl.database.IsLightOfRecord(this.db_hdr, this.db_rec);
}

function TOpenGLWnd_EnableLightOfRecord(flag)
{
	return gl.database.EnableLightOfRecord(this.db_hdr, this.db_rec, flag);
}

function TOpenGLWnd_IsBlendOfRecord()
{
	return gl.database.IsBlendOfRecord(this.db_hdr, this.db_rec);
}

function TOpenGLWnd_EnableBlendOfRecord(flag)
{
	return gl.database.EnableBlendOfRecord(this.db_hdr, this.db_rec, flag);
}

function TOpenGLWnd_IsVisibleOfRecord()
{
	return gl.database.IsVisibleOfRecord(this.db_hdr, this.db_rec);
}

function TOpenGLWnd_EnableVisibleOfRecord(flag)
{
	return gl.database.EnableVisibleOfRecord(this.db_hdr, this.db_rec, flag);
}

function TOpenGLWnd_EnablePrimitivePickupMode(flag)
{
	return gl.database.EnablePrimitivePickupMode(this.db_hdr, this.db_rec, flag);
}

function TOpenGLWnd_IsBosseyedOfRecord()
{
	return gl.database.IsBosseyedOfRecord(this.db_hdr, this.db_rec);
}

function TOpenGLWnd_EnableBosseyedOfRecord(flag)
{
	return gl.database.EnableBosseyedOfRecord(this.db_hdr, this.db_rec, flag);
}

function TOpenGLWnd_IsPlotModeOfRecord()
{
	return gl.database.IsPlotModeOfRecord(this.db_hdr, this.db_rec);
}

function TOpenGLWnd_EnablePlotModeOfRecord(flag)
{
	return gl.database.EnablePlotModeOfRecord(this.db_hdr, this.db_rec, flag);
}

TOpenGLRecord.prototype.GetKey = TOpenGLWnd_GetKeyOfGlRecord;
TOpenGLRecord.prototype.GetDisplayList = TOpenGLWnd_GetDisplayListIDOfRecord;
TOpenGLRecord.prototype.GetRenderMode = TOpenGLWnd_GetRenderModeOfRecord;
TOpenGLRecord.prototype.SetRenderMode = TOpenGLWnd_SetRenderModeOfRecord;
TOpenGLRecord.prototype.GetColor = TOpenGLWnd_GetColorOfRecord;
TOpenGLRecord.prototype.SetColor = TOpenGLWnd_SetColorOfRecord;
TOpenGLRecord.prototype.GetHightLightColor = TOpenGLWnd_GetHightLightColorOfRecord;
TOpenGLRecord.prototype.SetHightLightColor = TOpenGLWnd_SetHightLightColorOfRecord;
TOpenGLRecord.prototype.GetMarkColor = TOpenGLWnd_GetMarkColorOfRecord;
TOpenGLRecord.prototype.SetMarkColor = TOpenGLWnd_SetMarkColorOfRecord;
TOpenGLRecord.prototype.GetPolygonMode = TOpenGLWnd_GetPolygonModeOfRecord;
TOpenGLRecord.prototype.SetPolygonMode = TOpenGLWnd_SetPolygonModeOfRecord;
TOpenGLRecord.prototype.GetLineWidth = TOpenGLWnd_GetLineWidthOfRecord;
TOpenGLRecord.prototype.SetLineWidth = TOpenGLWnd_SetLineWidthOfRecord;
TOpenGLRecord.prototype.GetPointSize = TOpenGLWnd_GetPointSizeOfRecord;
TOpenGLRecord.prototype.SetPointSize = TOpenGLWnd_SetPointSizeOfRecord;
TOpenGLRecord.prototype.IsLight = TOpenGLWnd_IsLightOfRecord;
TOpenGLRecord.prototype.EnableLight = TOpenGLWnd_EnableLightOfRecord;
TOpenGLRecord.prototype.IsBlend = TOpenGLWnd_IsBlendOfRecord;
TOpenGLRecord.prototype.EnableBlend = TOpenGLWnd_EnableBlendOfRecord;
TOpenGLRecord.prototype.IsVisible = TOpenGLWnd_IsVisibleOfRecord;
TOpenGLRecord.prototype.EnableVisible = TOpenGLWnd_EnableVisibleOfRecord;
TOpenGLRecord.prototype.IsBosseyed = TOpenGLWnd_IsBosseyedOfRecord;
TOpenGLRecord.prototype.EnableBosseyed = TOpenGLWnd_EnableBosseyedOfRecord;
TOpenGLRecord.prototype.IsPlotMode = TOpenGLWnd_IsPlotModeOfRecord;
TOpenGLRecord.prototype.EnablePlotMode = TOpenGLWnd_EnablePlotModeOfRecord;
TOpenGLRecord.prototype.EnablePrimitivePickupMode = TOpenGLWnd_EnablePrimitivePickupMode;

function TOpenGLSensor(window_hdr)
{
	this.window_hdr = window_hdr;
}

/* 
TOpenGLSensor_Activate's parameters description.
-------------------------------------------------

0  - PAN_SENSOR,
1  - ROTATE_SENSOR,
2  - SCALE_SENSOR,
3  - RECT_SCALE_SENSOR,
4  - RESET_SENSOR,
5  - YOX_SENSOR,
6  - XOY_SENSOR,
7  - ZOY_SENSOR,
8  - YOZ_SENSOR,
9  - XOZ_SENSOR,
10 - ZOX_SENSOR

*/

function TOpenGLSensor_Activate(sensor_type)
{
	gl.database.SetSensorType(this.window_hdr, sensor_type);
}

TOpenGLSensor.prototype.Activate = TOpenGLSensor_Activate;

/*
 * TOpenglDatabase class wrap.
 */

function TOpenglDatabase(window_hdr, db_hdr)
{
	this.window_hdr = window_hdr;
	this.db_hdr = db_hdr;
	this.ogl_db = new TOpenGLWnd(window_hdr, db_hdr);
}

function TOpenglDatabase_Refresh()
{
	this.ogl_db.RefreshGlCanvas();
}

TOpenglDatabase.prototype.Refresh = TOpenglDatabase_Refresh;

function TOpenglDatabase_CreateRecord(type, sub_type)
{
	var tag_key = this.ogl_db.CreateKey(type, sub_type);

	if(this.ogl_db.IsExistRecord(tag_key) == true)
	{
		var db_rec = this.ogl_db.GetRecord(tag_key);
		var dl_id = this.ogl_db.GetDisplayListOfRecord(db_rec);
		this.ogl_db.ClearList(dl_id);
	}
	else
	{
		this.ogl_db.AppendRecord(tag_key);
	}

	var db_rec = this.ogl_db.GetRecord(tag_key);
	var obj_db_rec = new TOpenGLRecord(this.db_hdr, db_rec);

	return obj_db_rec;
}

TOpenglDatabase.prototype.CreateRecord = TOpenglDatabase_CreateRecord;

function TOpenglDatabase_DeleteAllRecords()
{
    this.ogl_db.DeleteAllRecords();
}

TOpenglDatabase.prototype.DeleteAllRecords = TOpenglDatabase_DeleteAllRecords;

function TOpenglDatabase_GetRecord(type, sub_type)
{
    var tag_key = this.ogl_db.CreateKey(type, sub_type);
    
    if(this.ogl_db.IsExistRecord(tag_key) == true)
    {
	var db_rec = this.ogl_db.GetRecord(tag_key);
	var obj_db_rec = new TOpenGLRecord(this.db_hdr, db_rec);

	return obj_db_rec;
    }
    else
    {
	return false;
    }
}

TOpenglDatabase.prototype.GetRecord = TOpenglDatabase_GetRecord;

function TOpenglDatabase_GetKey(type, sub_type)
{
    return this.ogl_db.CreateKey(type, sub_type);
}

TOpenglDatabase.prototype.GetKey = TOpenglDatabase_GetKey;

function TOpenglDatabase_Begin(obj_db_rec)
{
	var dl_id = obj_db_rec.GetDisplayList();
	this.ogl_db.BeginList(dl_id);
}

TOpenglDatabase.prototype.Begin = TOpenglDatabase_Begin;

function TOpenglDatabase_End()
{
	this.ogl_db.EndList();
}

TOpenglDatabase.prototype.End = TOpenglDatabase_End;

function TOpenglDatabase_BeginPrimitive(type)
{
	this.ogl_db.BeginPrimitive(type);
}

TOpenglDatabase.prototype.BeginPrimitive = TOpenglDatabase_BeginPrimitive;

function TOpenglDatabase_EndPrimitive()
{
	this.ogl_db.EndPrimitive();
}

TOpenglDatabase.prototype.EndPrimitive = TOpenglDatabase_EndPrimitive;

function TOpenglDatabase_InputVertex(x, y, z)
{
	this.ogl_db.InputVertex(x, y, z);
}

TOpenglDatabase.prototype.InputVertex = TOpenglDatabase_InputVertex;

function TOpenglDatabase_InputNormal(x, y, z)
{
	this.ogl_db.InputNormal(x, y, z);
}

TOpenglDatabase.prototype.InputNormal = TOpenglDatabase_InputNormal;


////////////////////////////////////////////////////////////////////////
// implement require function to be used to load js module as nodejs.

function require(js_file)
{
    var root_dir = comx.sys.GetEnvironmentVariables("COMX_SDK") + "js\\";
    var home_dir = comx.sys.GetEnvironmentVariables(unit.name.toUpperCase()) + "js\\";

    var js_path = "";

    if(comx.sys.IsFileExist(root_dir + js_file))
    {
	js_path = root_dir + js_file;
    }

    if(comx.sys.IsFileExist(home_dir + js_file))
    {
	js_path = home_dir + js_file;
    }

    if(js_path == "")
    {
	return false;
    }

    comx.sys.CreateFileHandler("js_module", js_path);
    var js_content = comx.file.js_module;
    comx.sys.CloseFileHandler("js_module");

    var js_cmd = "(function(){var exports = new Object();";
    js_cmd += js_content;
    js_cmd += "return exports;})();";

    return eval(js_cmd);
}

function Import(js_file)
{
    var root_dir = comx.sys.GetEnvironmentVariables("COMX_SDK") + "js\\";
    var home_dir = comx.sys.GetEnvironmentVariables(unit.name.toUpperCase()) + "js\\";

    var js_path = "";

    if(comx.sys.IsFileExist(root_dir + js_file))
    {
	js_path = root_dir + js_file;
    }

    if(comx.sys.IsFileExist(home_dir + js_file))
    {
	js_path = home_dir + js_file;
    }

    if(js_path == "")
    {
	return false;
    }

    comx.sys.CreateFileHandler("js_module", js_path);
    var js_content = comx.file.js_module;
    comx.sys.CloseFileHandler("js_module");

    return eval(js_content);
}

////////////////////////////////////////////////////////////////////////
// implement SetTimeout and KillTimeout functions

function SetTimeout(uElapse, fn)
{
    var nTimerID = comx.sys.SetTimeout(pane.formid, uElapse);
    timer_bf1bdc8f758c4965a586da93f27ac17a[nTimerID] = fn;

    return nTimerID;
}

var timer_bf1bdc8f758c4965a586da93f27ac17a = new Object();

function OnTimer_bf1bdc8f758c4965a586da93f27ac17a(nTimerID)
{
    if(typeof(timer_bf1bdc8f758c4965a586da93f27ac17a[nTimerID]) == 'function')
    {
	timer_bf1bdc8f758c4965a586da93f27ac17a[nTimerID](nTimerID);
    }
}

function KillTimeout(nTimerID)
{
    timer_bf1bdc8f758c4965a586da93f27ac17a[nTimerID] = false;
    comx.sys.KillTimeout(pane.formid, nTimerID);
}

////////////////////////////////////////////////////////////////////////
// implement PlayGif function

function PlayGif_(gif_fpath, uElapse, draw_cb)
{
    var nFrameNum = parseInt(widget.bitmap.ExtractGif(gif_fpath));

    var index1 = gif_fpath.lastIndexOf('/');
    var index2 = gif_fpath.lastIndexOf('\\');
    var index = (index1 > index2) ? index1 : index2;
    var gif_fname = gif_fpath.substr(index + 1);
    var iFrame = 1;

    return function()
    {
	var cache_dir = comx.sys.GetEnvironmentVariables("COMX_SDK") + "\\picture\\cache\\" + gif_fname + "." + iFrame + ".bmp";

	if(typeof(draw_cb) == 'function')
	    draw_cb(cache_dir);

	iFrame++;
	if(iFrame > nFrameNum)
	    iFrame = 1;
    };
}

function PlayGif(gif_fpath, uElapse, cb)
{
    return SetTimeout(uElapse, PlayGif_(gif_fpath, uElapse, cb));
}

function PlayGifRollBack_(gif_fpath, uElapse, draw_cb)
{
    var nFrameNum = parseInt(widget.bitmap.ExtractGif(gif_fpath));

    var index1 = gif_fpath.lastIndexOf('/');
    var index2 = gif_fpath.lastIndexOf('\\');
    var index = (index1 > index2) ? index1 : index2;
    var gif_fname = gif_fpath.substr(index + 1);
    var iFrame = 1;
    var bPlus = true;

    return function()
    {
	var cache_dir = comx.sys.GetEnvironmentVariables("COMX_SDK") + "\\picture\\cache\\" + gif_fname + "." + iFrame + ".bmp";

	if(typeof(draw_cb) == 'function')
	    draw_cb(cache_dir);

	if(bPlus)
	    iFrame++;
	else
	    iFrame--;

	if(iFrame > nFrameNum)
	{
	    iFrame = nFrameNum;
	    bPlus = false;
	}
	
	if(iFrame == 0)
	{
	    bPlus = true;
	    iFrame = 1;
	}
    };
}

function PlayGifRollBack(gif_fpath, uElapse, cb)
{
    return SetTimeout(uElapse, PlayGifRollBack_(gif_fpath, uElapse, cb));
}

function StopGif(nID)
{
    KillTimeout(nID);
}

function GetNilBitmap()
{
    return comx.sys.GetEnvironmentVariables("COMX_SDK") + "\\picture\\" + 'nil.bmp';
}

function GetGifFullPath(gif_fname)
{
    var fpath = comx.sys.GetEnvironmentVariables(unit.name.toUpperCase()) + "\\picture\\" + gif_fname;
    if(comx.sys.IsFileExist(fpath))
    {
	return fpath;
    }
    
    fpath = comx.sys.GetEnvironmentVariables("COMX_SDK") + "\\picture\\" + gif_fname;
    if(comx.sys.IsFileExist(fpath))
    {
	return fpath;
    }

    return false;
}

function alert(str)
{
    
    comx.gf.var54F11ACB79804f16B029E726E3044685 = str;
    comx.ui.PopupSystemModalForm('0x5e4014ec-0x5046-0x464f-0xae-0x77-0xe3-0x4c-0x71-0x41-0x20-0xd1');
}
