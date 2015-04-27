#include <time.h>   
//#include <curses.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

//client code

char * user;// = (char*)malloc(20*sizeof(char));
char * password;// = (char*)malloc(20*sizeof(char));
char * host;// =  (char*)malloc(20*sizeof(char));
char * sport;// = (char*)malloc(20*sizeof(char));
//char *command = (char*)malloc(20*sizeof(char));
int port;
char *room;
GtkTreeSelection *selection;
enum
{
  LIST_ITEM = 0,
  N_COLUMNS
};
//void refresh_msg(int);
//void send_message(( GtkWidget , gpointer));                    
int open_client_socket(char * host, int port) {
        // Initialize socket address structure
        struct  sockaddr_in socketAddress;
        
        // Clear sockaddr structure
        memset((char *)&socketAddress,0,sizeof(socketAddress));
                
        // Set family to Internet
        socketAddress.sin_family = AF_INET;
        
        // Set port
        socketAddress.sin_port = htons((u_short)port);
                
        // Get host table entry for this host
        struct  hostent  *ptrh = gethostbyname(host);
        if ( ptrh == NULL ) {
                perror("gethostbyname");
                exit(1);
        }

        // Copy the host ip address to socket address structure
        memcpy(&socketAddress.sin_addr, ptrh->h_addr, ptrh->h_length);
        
        // Get TCP transport protocol entry
        struct  protoent *ptrp = getprotobyname("tcp");
        if ( ptrp == NULL ) {
                perror("getprotobyname");
                exit(1);
        }
        
        // Create a tcp socket
        int sock = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
        if (sock < 0) {
                perror("socket");
                exit(1);
        }
        
        // Connect the socket to the specified server
        if (connect(sock, (struct sockaddr *)&socketAddress,
                    sizeof(socketAddress)) < 0) {
                perror("connect");
                exit(1);
        }
        
        return sock;
}
                
#define MAX_RESPONSE (10 * 1024)
int sendCommand(char *  host, int port, char * command, char * response) {
        
        int sock = open_client_socket( host, port);
        
        if (sock<0) {
                return 0;
        }
                
        // Send command 
        write(sock, command, strlen(command));
        write(sock, "\r\n",2);
        
        //Print copy to stdout
        write(1, command, strlen(command));
        write(1, "\r\n",2);
                
        // Keep reading until connection is closed or MAX_REPONSE
        int n = 0;
        int len = 0;
        while ((n=read(sock, response+len, MAX_RESPONSE - len))>0) {
                len += n;
        }
        response[len]=0;
         
        printf("response:\n%s\n", response);
        
        close(sock);
        
        return 1;
}

void
printUsage()
{
        printf("Usage: test-talk-server host port command\n");
        exit(1);
}       
        


//client code




GtkListStore * list_rooms;
GtkListStore * list_users;
void clear_list_rooms() {
GtkTreeIter iter;
               //gchar *msg = g_strdup_printf ("%s", room1);
       // gtk_list_store_append (GTK_LIST_STORE (list_rooms), &iter);
        //gtk_list_store_set (GTK_LIST_STORE (list_rooms),
         gtk_list_store_clear(GTK_LIST_STORE (list_rooms));
}
/*
void refresh_msg(int i) {
      char * command = (char*)malloc(1000*sizeof(char));

        strcpy(command,"GET-MESSAGES");
        char *a = command;
        while(*a != '\0') {
        a++;
        }
        *a = ' ';
        a++;
        strcpy(a,user);
        while(*a != '\0') a++;
        *a = ' ';
        a++;
        strcpy(a,password);

        while(*a != '\0') a++;
        *a = ' ';
        a++;
         char *str = (char*)malloc(15*sizeof(char));
        sprintf(str, "%d", i);//check for possible bug
        strcpy(a,str);
        while(*a != '\0') a++;
        *a = ' ';
        a++;

        strcpy(a,room);
        char *response= (char*)malloc(MAX_RESPONSE*sizeof(char));
        sendCommand(host, port, command, response);

        create_text(response);




}
*/
  void update_list_users() { 
	if(room != NULL) {
	 GtkTreeIter iter;
    int i;
       char * command = (char*)malloc(1000*sizeof(char));
        
        strcpy(command,"GET-USERS-IN-ROOM");
        char *a = command;
        while(*a != '\0') {
        a++;
        }
        *a = ' ';
        a++;
        strcpy(a,user);
        while(*a != '\0') a++;
        *a = ' ';
        a++;
        strcpy(a,password);

        while(*a != '\0') a++;
        *a = ' ';
        a++;
        strcpy(a,room);


        char *response= (char*)malloc(MAX_RESPONSE*sizeof(char));
        sendCommand(host, port, command, response);
   
        while(*response != '\0') {
        
        char * room1 = (char*)malloc(100*sizeof(char)); 
        char * a1 = room1;
        while(*response != '\r') { 
        *room1 = *response;
        room1++;
        response++;
        }
        *room1 = '\0';
        room1 = a1;
        response++;
        response++;
                gchar *msg = g_strdup_printf ("%s", room1);
        gtk_list_store_append (GTK_LIST_STORE (list_rooms), &iter);
        gtk_list_store_set (GTK_LIST_STORE (list_rooms),
                            &iter,
                            0, msg,
                            -1);
        g_free (msg);
    
   
    }


}
}
  void update_list_rooms() {  GtkTreeIter iter;
    int i;
       char * command = (char*)malloc(1000*sizeof(char));
	
	strcpy(command,"LIST-ROOMS");
	char *a = command;
	while(*a != '\0') {
	a++;
	}	
	*a = ' ';
	a++;
	strcpy(a,user);
	while(*a != '\0') a++;
	*a = ' ';
	a++;
	strcpy(a,password);

//	while(1){	
	char *response= (char*)malloc(MAX_RESPONSE*sizeof(char));
        sendCommand(host, port, command, response);
		
	while(*response != '\0') {
		
	char * room1 = (char*)malloc(100*sizeof(char));
	char * a1 = room1;
	while(*response != '\r') {
	*room1 = *response;
	room1++;
	response++;
	}
	*room1 = '\0';
	room1 = a1;
	response++;
	response++;
	        gchar *msg = g_strdup_printf ("%s", room1);
        gtk_list_store_append (GTK_LIST_STORE (list_rooms), &iter);
        gtk_list_store_set (GTK_LIST_STORE (list_rooms),
                            &iter, 
                            0, msg,
                            -1);
        g_free (msg);
//	sleep(5);


	
//	}


}
    /* Add some messages to the window */
   /* for (i = 0; i < 10; i++) {
        gchar *msg = g_strdup_printf ("Room %d", i);
        gtk_list_store_append (GTK_LIST_STORE (list_rooms), &iter);
        gtk_list_store_set (GTK_LIST_STORE (list_rooms), 
	                    &iter,
                            0, msg,
	                    -1);
	g_free (msg);
    }

*/
}

/* Create the list of "messages" */
static GtkWidget *create_list( const char * titleColumn, GtkListStore *model )
{
    GtkWidget *scrolled_window;
    GtkWidget *tree_view;
    //GtkListStore *model;
    GtkCellRenderer *cell;
    GtkTreeViewColumn *column;

    int i;
   int lmsg;  //last message received
    /* Create a new scrolled window, with scrollbars only if needed */
    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
				    GTK_POLICY_AUTOMATIC, 
				    GTK_POLICY_AUTOMATIC);
   
    //model = gtk_list_store_new (1, G_TYPE_STRING);
    tree_view = gtk_tree_view_new ();
    gtk_container_add (GTK_CONTAINER (scrolled_window), tree_view);
    gtk_tree_view_set_model (GTK_TREE_VIEW (tree_view), GTK_TREE_MODEL (model));
    gtk_widget_show (tree_view);
/*
//new code   
	
	       char * command = (char*)malloc(1000*sizeof(char));
        
        strcpy(command,"GET-MESSAGES");
        char *a = command;
        while(*a != '\0') {
        a++;
        }
        *a = ' ';
        strcpy(a,user);
        while(*a != '\0') a++;
        *a = ' ';
        strcpy(a,password);
 	while(*a != '\0') a++;
	*a = ' ';
	char str[15];
	sprintf(str, "%d", lmsg);
	strcpy(a,str);
	while(*a != '\0') a++;
	
	*a = ' ';
	strcpy(a,room); //after room is selected
	
	
        char *response= (char*)malloc(MAX_RESPONSE*sizeof(char));
        sendCommand(host, port, command, response);
 
        
	while(*response != '\0') {
        response++;  //skip over message number
	response++;
        char * msg1 = (char*)malloc(100*sizeof(char));
        char * a1 = msg1;
        while(*response != '\r') {
        
	*msg1 = *response;
        msg1++;
        response++;
        }
        *msg1 = '\0';
        msg1 = a1;
        response++;  
        response++;

	
//	        gchar *msg = g_strdup_printf ("%d", msg1);
  //      gtk_list_store_append (GTK_LIST_STORE (model), &iter);
    //    gtk_list_store_set (GTK_LIST_STORE (model),
      //                      &iter,   
        //                    0, msg,
          //                  -1);
      //  g_free (msg);

	
	
	}


//new code
  
*/
  cell = gtk_cell_renderer_text_new ();

    column = gtk_tree_view_column_new_with_attributes (titleColumn,
                                                       cell,
                                                       "text", 0,
                                                       NULL);
  
    gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view),
	  		         GTK_TREE_VIEW_COLUMN (column));

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
	  return scrolled_window;
}
   
/* Add some text to our text widget - this is a callback that is invoked
when our window is realized. We could also force our window to be
realized with gtk_widget_realize, but it would have to be part of
a hierarchy first */


GtkTextBuffer *buffer;
GtkTextBuffer *buffer1;
//
/*
static gboolean
time_handler(GtkWidget *widget)
{
clear_list_rooms();
  update_list_rooms();
refresh_msg(0);
return TRUE;
}
*/
//
static void insert_text( GtkTextBuffer *buffer, const char * initialText )
{
   GtkTextIter iter;
 
   gtk_text_buffer_get_iter_at_offset (buffer, &iter, 0);
//   gtk_text_buffer_insert (buffer, &iter, initialText,-1);
gtk_text_buffer_set_text(buffer, initialText, -1);

}

static void append_text(GtkTextBuffer *buffer, const char *initialText ) {
   GtkTextIter iter;
   
   gtk_text_buffer_get_iter_at_offset (buffer, &iter, 0);
   gtk_text_buffer_insert (buffer, &iter, initialText,-1);
//gtk_text_buffer_set_text(buffer, initialText, -1);
 


}

   
/* Create a scrolled text area that displays a "message" */
static GtkWidget *create_text( const char * initialText )
{
   GtkWidget *scrolled_window;
   GtkWidget *view;
   //GtkTextBuffer *buffer;

   view = gtk_text_view_new ();
   buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

   scrolled_window = gtk_scrolled_window_new (NULL, NULL);
   gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
		   	           GTK_POLICY_AUTOMATIC,
				   GTK_POLICY_AUTOMATIC);

   gtk_container_add (GTK_CONTAINER (scrolled_window), view);
   insert_text (buffer, initialText);

   gtk_widget_show_all (scrolled_window);

   return scrolled_window;
}

static GtkWidget *create_text1( const char * initialText ) 
{
   GtkWidget *scrolled_window;
   GtkWidget *view;
   //GtkTextBuffer *buffer;
        
   view = gtk_text_view_new ();
   buffer1 = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
        
   scrolled_window = gtk_scrolled_window_new (NULL, NULL);
   gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                   GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_AUTOMATIC);
        
   gtk_container_add (GTK_CONTAINER (scrolled_window), view);
   insert_text (buffer, initialText);
        
   gtk_widget_show_all (scrolled_window);
         
   return scrolled_window;
}



void refresh_msg(int i) {
//      GtkTextBuffer *buffer;
//	GtkWidget *view;
  // view = gtk_text_view_new ();
  // buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
	GtkWidget *messages;
	char * command = (char*)malloc(1000*sizeof(char));
        
        strcpy(command,"GET-MESSAGES");
        char *a = command;
        while(*a != '\0') {
        a++;
        }
        *a = ' ';
        a++;
        strcpy(a,user);
        while(*a != '\0') a++;  
        *a = ' ';
        a++;
        strcpy(a,password);
	
	while(*a != '\0') a++;
        *a = ' ';
        a++;
    	 char *str = (char*)malloc(15*sizeof(char));
        sprintf(str, "%d", i);//check for possible bug
	strcpy(a,str);
	while(*a != '\0') a++;
        *a = ' ';
        a++;
	
	strcpy(a,room);
	char *response= (char*)malloc(MAX_RESPONSE*sizeof(char));
        sendCommand(host, port, command, response);
//gtk_text_buffer_set_text(buffer,response,-1);

int j = 0;
/*
while(*response != '\0') {
char * mymsg = (char*)malloc(200*sizeof(char));
char *temp = mymsg;
response++;
if(response != '\0') response++;
while(*response != '\r') {
*mymsg = *response;
mymsg++;
if(response != '\0') response++;
}
if(response != '\0') response++;
if(response != '\0') response++;

//*mymsg = '\n';
//mymsg++;
*mymsg = '\0';
mymsg = temp;

if(j>=i) append_text(buffer,(const char*)mymsg);

j++;
//free(mymsg);
//free(temp);

}
*/	
	insert_text(buffer,response);
//	messages = create_text(response);	
//    gtk_table_attach_defaults (GTK_TABLE (table), messages, 0, 4, 2, 5);
  //  gtk_widget_show (messages);	
//gtk_text_buffer_insert(buffer, 0, response,-1);	
	
}

void  on_changed(GtkWidget *widget, gpointer label) 
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  char *value;
  if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter)) {

    gtk_tree_model_get(model, &iter, 0, &value,  -1);
   // gtk_label_set_text(GTK_LABEL(label), value);
 	
if(room != NULL) {
if(strcmp(room,value)==0)
        return;
}
	if(room != NULL) {
	char * command3 = (char*)malloc(1000*sizeof(char));
        strcpy(command3,"LEAVE-ROOM");
        char *c = command3; 
        while(*c != '\0') {
        c++;
        }
        *c = ' ';
        c++;
        strcpy(c,user);
        while(*c != '\0') c++;
        *c = ' ';
        c++;
        strcpy(c,password);
        
        while(*c != '\0') c++;
        *c = ' ';
        c++;
        strcpy(c,room);
       // strcpy(a,str);
        char *response3= (char*)malloc(MAX_RESPONSE*sizeof(char));
        sendCommand(host, port, command3, response3);


	//send: user has left room

	char * command4 = (char*)malloc(1000*sizeof(char));
        strcpy(command4,"SEND-MESSAGE");
        char *d = command4;
        while(*d != '\0') {
        d++;
        }
        *d = ' ';
        d++;
        strcpy(d,user);
        while(*d != '\0') d++;
        *d = ' ';
        d++;
        strcpy(d,password);
        
        while(*d != '\0') d++;
        *d = ' ';
        d++;
        strcpy(d,room);
            
	while(*d != '\0') d++;
        *d = ' ';
        d++;
       strcpy(d,": has left the room");
       // strcpy(a,str);
        char *response4= (char*)malloc(MAX_RESPONSE*sizeof(char));
        sendCommand(host, port, command4, response4);  
        

	
	
	
	
}  	
	room = strdup(value);
	//printf("%s",room);

	               char * command = (char*)malloc(1000*sizeof(char));

        strcpy(command,"ENTER-ROOM");
        char *a = command;
        while(*a != '\0') {
        a++;
        }
        *a = ' ';
        a++;
        strcpy(a,user);
        while(*a != '\0') a++;
        *a = ' ';
        a++;  
        strcpy(a,password);
        
        while(*a != '\0') a++;
        *a = ' ';
        a++;
        strcpy(a,room);
        
       
       // strcpy(a,str);
        char *response= (char*)malloc(MAX_RESPONSE*sizeof(char));
        sendCommand(host, port, command, response);
		
	//send message that user has entered room
	char * command1 = (char*)malloc(1000*sizeof(char));
        
        strcpy(command1,"SEND-MESSAGE");
        char *b = command1;
        while(*b != '\0') {
        b++;
        }
        *b = ' ';
        b++;
        strcpy(b,user);
        while(*b != '\0') b++;  
        *b = ' ';
        b++;
        strcpy(b,password);

	while(*b != '\0') b++;
        *b = ' ';
        b++;
        strcpy(b,room);

        while(*b != '\0') b++;
        *b = ' ';
        b++;
        // char *str = (char*)malloc(15*sizeof(char));
       // sprintf(str, "%s: entered room ", user);//check for possible bug
        strcpy(b,": has entered room"); 
	char *response1= (char*)malloc(MAX_RESPONSE*sizeof(char));
        sendCommand(host, port, command1, response1);

	insert_text(buffer,"");	
	refresh_msg(0);	
	


	 g_free(value);
  }

}

static gboolean
time_handler(GtkWidget *widget)
{
clear_list_rooms();
  update_list_rooms();
if(room != NULL) 
refresh_msg(0);
return TRUE;
}


//callback for send button

void send_message( GtkWidget *widget, gpointer   data ) {
GtkTextIter iter1;
GtkTextIter iter2;
gtk_text_buffer_get_start_iter(buffer1, &iter1);
gtk_text_buffer_get_end_iter(buffer1,&iter2);

gchar *mess =   gtk_text_buffer_get_text(buffer1, &iter1, &iter2, FALSE);

char * mess1 = (char*) mess;

        char * command1 = (char*)malloc(1000*sizeof(char));
 
        strcpy(command1,"SEND-MESSAGE");
        char *b = command1;
        while(*b != '\0') {
        b++;
        }
        *b = ' ';
        b++;
        strcpy(b,user);
        while(*b != '\0') b++; 
        *b = ' ';
        b++;
        strcpy(b,password);

        while(*b != '\0') b++;
        *b = ' ';
        b++;
        strcpy(b,room);
                
        while(*b != '\0') b++;
        *b = ' ';
        b++;
        // char *str = (char*)malloc(15*sizeof(char));
       // sprintf(str, "%s: entered room ", user);//check for possible bug
        strcpy(b,mess1);
        char *response1= (char*)malloc(MAX_RESPONSE*sizeof(char));
        sendCommand(host, port, command1, response1);

	insert_text(buffer1,"");






}
int main( int   argc,
          char *argv[] )
{


	//char response[MAX_RESPONSE];
        //sendCommand(host, port, command, response);
            if (argc < 4) {
                printUsage();
        }
        
        host = argv[1];
        sport = argv[2];
        user = argv[3];
         password = argv[4];
        sscanf(sport, "%d", &port);
         



        //char response[MAX_RESPONSE];
        //sendCommand(host, port, command, response);

    GtkWidget *window;
    GtkWidget *list;
    GtkWidget *messages;
    GtkWidget *myMessage;
//	GTKWidget *userlist;
//    GtkTreeSelection *selection;
GtkWidget *label;
    gtk_init (&argc, &argv);
   

label = gtk_label_new("");
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "Paned Windows");
    g_signal_connect (window, "destroy",
	              G_CALLBACK (gtk_main_quit), NULL);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    gtk_widget_set_size_request (GTK_WIDGET (window), 450, 400);

    // Create a table to place the widgets. Use a 7x4 Grid (7 rows x 4 columns)
    GtkWidget *table = gtk_table_new (7, 4, TRUE);
    gtk_container_add (GTK_CONTAINER (window), table);
    gtk_table_set_row_spacings(GTK_TABLE (table), 5);
    gtk_table_set_col_spacings(GTK_TABLE (table), 5);
    gtk_widget_show (table);

    // Add list of rooms. Use columns 2 to 4 (exclusive) and rows 0 to 2 (exclusive)
    list_rooms = gtk_list_store_new (1, G_TYPE_STRING);
    update_list_rooms();
//  g_timeout_add_seconds(5,update_list_rooms(),NULL);
	  list = create_list ("Rooms", list_rooms);
    gtk_table_attach_defaults (GTK_TABLE (table), list, 2, 4, 0, 2);
    gtk_widget_show (list);


//Add list of users
    // Add list of rooms. Use columns 0 to 2 (exclusive) and rows 0 to 2 
    list_users = gtk_list_store_new (1, G_TYPE_STRING);
    update_list_users();
//  g_timeout_add_seconds(5,update_list_rooms(),NULL);
          list = create_list ("Users", list_rooms);
    gtk_table_attach_defaults (GTK_TABLE (table), list, 0, 2, 0, 2);
    gtk_widget_show (list);




//selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_rooms));   
    // Add messages text. Use columns 0 to 4 (exclusive) and rows 4 to 7 (exclusive) 
    messages = create_text ("");
    gtk_table_attach_defaults (GTK_TABLE (table), messages, 0, 4, 2, 5);
    gtk_widget_show (messages);
    // Add messages text. Use columns 0 to 4 (exclusive) and rows 4 to 7 (exclusive) 

    myMessage = create_text1 ("");
    gtk_table_attach_defaults (GTK_TABLE (table), myMessage, 0, 4, 5, 7);
    gtk_widget_show (myMessage);

    // Add send button. Use columns 0 to 1 (exclusive) and rows 4 to 7 (exclusive)
    GtkWidget *send_button = gtk_button_new_with_label ("Send");
    gtk_table_attach_defaults(GTK_TABLE (table), send_button, 0, 1, 7, 8); 
    gtk_widget_show (send_button);
    
    gtk_widget_show (table);
    gtk_widget_show (window);
//g_timeout_add_seconds(10, callback, "callback_function");
//	g_timeout_add_seconds(5, update_list_rooms(),(gpointer)NULL);

g_timeout_add(5000, (GSourceFunc) time_handler, (gpointer) window);

//callbacks
    g_signal_connect (send_button, "clicked", G_CALLBACK (send_message), NULL);

  g_signal_connect(selection, "changed",  G_CALLBACK(on_changed), label); // when room is changed 	


//  while(1) {
//update_list_rooms();
//sleep(5);
//}
	  gtk_main ();

    return 0;
}

