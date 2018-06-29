IF EXISTS (SELECT * FROM sysobjects WHERE type = 'V' AND name = 'View_Name')
	BEGIN
		PRINT 'Dropping View View_Name'
		DROP  View View_Name
	END
GO

/******************************************************************************
**		File: 
**		Name: View_Name
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

PRINT 'Creating View View_Name'
GO
CREATE View View_Name
as

	/*INSERT_SELECT_STATEMENT*/


GO


GRANT SELECT ON View_Name TO PUBLIC

GO
