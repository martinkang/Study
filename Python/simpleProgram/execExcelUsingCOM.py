import win32com.client

# COM( Component Object Model ) 을 이용하여 엑셀 다루기
sExcel = win32com.client.Dispatch( "Excel.Application")
sExcel.Visible = True

sWorkBook = sExcel.Workbooks.Add()
sWorkSheet = sWorkBook.Worksheets( "Sheet1" )

sWorkSheet.Cells( 1, 1 ).Value = "hello world"
sWorkBook.SaveAs( 'test.xlsx' )

sExcel.Quit()
