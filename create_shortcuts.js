//--------------------------------------------
var WshShell = new ActiveXObject("WScript.Shell");
var FileSystemObject = new ActiveXObject("Scripting.FileSystemObject");
var WScript = new Object;
WScript.CreateObject = function CreateObject(objectType){ return new ActiveXObject(objectType); };
//--------------------------------------------
function WriteBufferToFile(buffer, filename){
	var file = FileSystemObject.CreateTextFile(filename, true);
	file.Write(buffer);
	file.Close();
}
//--------------------------------------------
function ReplaceASCIIString(fileName){
	var file = FileSystemObject.OpenTextFile(fileName, 1); // 1 = read
	var buffer="";

	for (var iArg = 1; iArg < ReplaceASCIIString.arguments.length; iArg+=2){
		var reg=new RegExp("\\$"+ReplaceASCIIString.arguments[iArg]+"\\$", "g");
		ReplaceASCIIString.arguments[iArg]=reg;
		ReplaceASCIIString.arguments[iArg+1]=ReplaceASCIIString.arguments[iArg+1].replace("\\","\\\\");
	}

	while(!file.AtEndOfStream){
		var line=file.ReadLine();
		for (var iArg = 1; iArg < ReplaceASCIIString.arguments.length; iArg+=2)
			line=line.replace(ReplaceASCIIString.arguments[iArg], ReplaceASCIIString.arguments[iArg+1]);
		buffer += line;	
		buffer+="\r\n";
	}
	file.Close();
	return buffer;
}
//--------------------------------------------
function Install_VC6(){
	var RegData=ReplaceASCIIString("VisualUltimate\\VS_Config\\DevStudio#6.0.hjson", "INSTALLDIR", WshShell.CurrentDirectory);
	WriteBufferToFile(RegData, "VisualUltimate\\VS_Config\\DevStudio#6.0.hjson");
}
//----------------------------------------------------------- 
function DirectoryExists(folder){
	return FileSystemObject.FolderExists(folder);
}
//--------------------------------------------
function CreateDirectory(dirName){
	var ParentDir=FileSystemObject.GetParentFolderName(dirName);
	if (!DirectoryExists(ParentDir))
		CreateDirectory(ParentDir);
	FileSystemObject.CreateFolder(dirName);
}
//--------------------------------------------
function IsDefined(variable){
	return !(variable==null || typeof(variable)=="undefined");
}
//----------------------------------------------------------- 
function Shortcut(LnkFile, Target, Arguments, WorkingDirectory, IconFile, iIcon, Description, Hotkey){
	var ParentDir=FileSystemObject.GetParentFolderName(LnkFile);
	if (!DirectoryExists(ParentDir))
		CreateDirectory(ParentDir);

	var link = WshShell.CreateShortcut(LnkFile);
	if (IsDefined(Arguments))
		link.Arguments = Arguments;
	if (IsDefined(Description))
		link.Description = Description;
	if (IsDefined(Hotkey))
		link.HotKey = Hotkey;//"CTRL+ALT+SHIFT+X";
	if (IsDefined(IconFile)){
		if (iIcon=="")
			iIcon=0;
		link.IconLocation = IconFile+","+iIcon;
	}
	link.TargetPath = Target;
	link.WindowStyle = 1;
	if (IsDefined(WorkingDirectory))
		link.WorkingDirectory = WorkingDirectory;
	link.Save();	
}
//----------------------------------------------------------- 
function CreateShortcuts(){
	var CurDir=WshShell.CurrentDirectory;
	var MsDevDir=CurDir+"\\COMMON\\MSDev98";
	var MsDevEXE=CurDir+"\\Common\\MSDev98\\Bin\\MSDev.exe";
	// --------------------------
	Shortcut(CurDir+"\\Visual C++ 6.0.lnk", MsDevEXE, "/config VC6.hjson", null, CurDir+"\\VisualUltimate\\Icons\\vc6.ico");
	Shortcut(CurDir+"\\Visual C++ 7.0.lnk", MsDevEXE, "/config VC7.hjson", null, MsDevEXE);
}
//----------------------------------------------------------- 
Install_VC6();
CreateShortcuts();