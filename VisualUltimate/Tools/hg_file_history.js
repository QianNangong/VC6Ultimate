//Q:\@Projets\SpiropsEditor>thgw.exe --nofork log --listfile "C:\tmp\THGFB82.tmp"
var WshShell = new ActiveXObject("WScript.Shell");
var FileSystemObject = new ActiveXObject("Scripting.FileSystemObject");
//var Environment = WshShell.Environment;
//--------------------------------------------
function Exec(cmdLine){
	var result=WshShell.Run(cmdLine,1,true);
	return result;
}
//--------------------------------------------
function CreateTempFile(){
	var TmpFile = FileSystemObject.GetSpecialFolder(2).Path+"\\"+FileSystemObject.GetTempName();
	var f = FileSystemObject.CreateTextFile(TmpFile, true);
	return f;
}
//--------------------------------------------
//WshShell.Run("ApplyEnvironmentChanges.exe", 0, false);
//--------------------------------------------
if (WScript.Arguments.length){
	var TmpFile = FileSystemObject.GetSpecialFolder(2).Path+"\\"+FileSystemObject.GetTempName();
	var fileName = WScript.Arguments(0);
	{
		var f = FileSystemObject.CreateTextFile(TmpFile, true);
		f.WriteLine(fileName);
		f.Close();
	}
	Exec("thgw.exe --nofork log --listfile \""+TmpFile+"\"");
}
//--------------------------------------------
