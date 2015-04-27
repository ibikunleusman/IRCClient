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

void clear_list_rooms() {
GtkTreeIter iter;
               //gchar *msg = g_strdup_printf ("%s", room1);
       // gtk_list_store_append (GTK_LIST_STORE (list_rooms), &iter);
        //gtk_list_store_set (GTK_LIST_STORE (list_rooms),
         gtk_list_store_clear(GTK_LIST_STORE (list_rooms));
}

void update_list_rooms() {
    GtkTreeIter iter;
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

//
static gboolean
time_handler(GtkWidget *widget)
{
clear_list_rooms();
  update_list_rooms();
return TRUE;
}
//
static void insert_text( GtkTextBuffer *buffer, const char * initialText )
{
   GtkTextIter iter;
 
   gtk_text_buffer_get_iter_at_offset (buffer, &iter, 0);
   gtk_text_buffer_insert (buffer, &iter, initialText,-1);
}
   
/* Create a scrolled text area that displays a "message" */
static GtkWidget *create_text( const char * initialText )
{
   GtkWidget *scrolled_window;
   GtkWidget *view;
   GtkTextBuffer *buffer;

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

void  on_changed(GtkWidget *widget, gpointer label) 
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  char *value;


  if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter)) {

    gtk_tree_model_get(model, &iter, 0, &value,  -1);
   // gtk_label_set_text(GTK_LABEL(label), value);
   	
	room = strdup(value);
	 g_free(value);
  }

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

    // Add list of rooms. Use columns 0 to 4 (exclusive) and rows 0 to 4 (exclusive)
    list_rooms = gtk_list_store_new (1, G_TYPE_STRING);
    update_list_rooms();
//  g_timeout_add_seconds(5,update_list_rooms(),NULL);
	  list = create_list ("Rooms", list_rooms);
    gtk_table_attach_defaults (GTK_TABLE (table), list, 2, 4, 0, 2);
    gtk_widget_show (list);
//selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_rooms));   
    // Add messages text. Use columns 0 to 4 (exclusive) and rows 4 to 7 (exclusive) 
    messages = create_text ("Peter: Hi how are you\nMary: I am fine, thanks and you?\nPeter: Fine thanks.\n");
    gtk_table_attach_defaults (GTK_TABLE (table), messages, 0, 4, 2, 5);
    gtk_widget_show (messages);
    // Add messages text. Use columns 0 to 4 (exclusive) and rows 4 to 7 (exclusive) 

    myMessage = create_text ("I am fine, thanks and you?\n");
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

  g_signal_connect(selection, "changed",  G_CALLBACK(on_changed), label); // when room is changed 	


//  while(1) {
//update_list_rooms();
//sleep(5);
//}
	  gtk_main ();

    return 0;
}

