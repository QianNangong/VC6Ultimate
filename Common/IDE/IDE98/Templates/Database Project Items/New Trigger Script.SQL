IF EXISTS (SELECT * FROM sysobjects WHERE type = 'TR' AND name = 'Trigger_Name')
	BEGIN
		PRINT 'Dropping Trigger Trigger_Name'
		DROP  Trigger Trigger_Name
	END
GO

/******************************************************************************
**		File: 
**		Name: Trigger_Name
**		Desc: 
**
**		This template can be customized:
**              
**
**		Auth: 
**		Date: 
*******************************************************************************
**		Change History
*******************************************************************************
**		Date:		Author:				Description:
**		--------		--------				-------------------------------------------
**    
*******************************************************************************/

PRINT 'Creating Trigger Trigger_Name'
GO
CREATE Trigger Trigger_Name 

   
	/*INSERT_TRIGGER_STATEMENT*/

GO

