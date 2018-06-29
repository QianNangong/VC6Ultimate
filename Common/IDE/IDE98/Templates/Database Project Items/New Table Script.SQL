IF EXISTS (SELECT * FROM sysobjects WHERE type = 'U' AND name = 'Table_Name')
	BEGIN
		PRINT 'Dropping Table Table_Name'
		DROP  Table Table_Name
	END
GO

/******************************************************************************
**		File: 
**		Name: Table_Name
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

PRINT 'Creating Table Table_Name'
GO
CREATE TABLE Table_Name
(
   


)
GO

GRANT SELECT ON Table_Name TO PUBLIC

GO
