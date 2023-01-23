21CS60D04
MAJOR KUNAL PILLEY
1. Only use either a positive or negative index at a time. One positive one negative not to be used.
2. Start and end index should be in increasing order : eg : (10,20)  (30,50)( -100,- 30) (-250, -1)       [ start_index< end_index ] . both should be positive or both should be negative index
3. The files should not have newline character after the last line - otherwise 1 additional line will be added if data inserted at end, 
file from assignment 1 - server.txt has been used as the model file. Also being given in the folder for testing.
4.  The client and server code should be kept in different folders for execution.
5. Always put ‘/’ for all the commands including yes and no response as well.
6. For inserting multiple lines at once use \n  to represent a newline character. Example :
“Line 1\nLine 2\nLine 3” will be treated as :
Line 1
Line 2
Line 3


7. Any client getting two requests will be addressed in order of reception.
8. If a file is blank it will not be downloaded but the client will be informed .
9. If a file is blank , data will be entered on index zero even if the data is provided with any index or no index , so that the client does not lose the data.