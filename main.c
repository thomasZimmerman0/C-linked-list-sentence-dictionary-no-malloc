#include <stdio.h>
#include <string.h>

typedef struct node { 
    char word[100];
    char lookup[5];
    int length; 
    struct node *next; 
}node; 

#define MAX_NODES 999

int case_sensitive = 1;

node * create_node( int a, char word[100] ){ 
    // Memory space to put your nodes. Note that is is just a MAX_NODES * sizeof( node ) memory array.
    static node node_pool[ MAX_NODES ];
    static int  next_node = 0;

    if ( next_node >= MAX_NODES )
    {
        printf( "Out of memory!\n" );
        return ( node * )NULL;
    }

    //Allocates space for the node
    node * n = &( node_pool[ next_node++ ] );

    //copies the word passed in by the user into the new node
    strcpy(n->word, word);
    //copies 4 letters for the lookup fucntion
    strncpy(n->lookup, n->word, 4);
    //sets length
    n->length = a;
    //sets next node pointer
    n->next = NULL;

    return n; 
}

node * create_dictionary(){
    // takes in a string up to 1001 characters that can include spaces
    char string[1001] = {};
    printf("Enter a sentence than press enter!:\n");
    scanf(" %[^\n]*c", string);


    //Sorts the above string into a multi-dimensional array so the words can be displayed and manipulated singularly
    int i, j, ctr, count;
    char words[100][100];
    char tempWords[100];

    j=0; ctr=0;
    for(i = 0; i < (strlen(string)); i++){
      
      if(string[i] == ' ' || string[i] == '\0'){
        words[ctr][j]='\0';
        ctr++;
        j=0;
      } else {
        words[ctr][j] = string[i];
        j++;
      }
    }

    //Alphabetically sorts the words 
    for(i=0;i<=ctr;i++){
      for(j=i+1;j<=ctr;j++){
         if(strcasecmp(words[i],words[j])>0){
            strcpy(tempWords,words[i]);
            strcpy(words[i],words[j]);
            strcpy(words[j],tempWords);
         }
      }
    }

    //Begins process of making linked list
    node *newtemp, *root, *temp; 

    //Creates root node with the initial word in the words array
    root = create_node( (strlen(words[0])), words[0] ); 
    temp = root; 

  //Generates a new node in each loop iteration, checks if the length is 0 to ensure no nodes without words are not being created, sets the current node to point to the new node, then the current node to the new node.
  for(i = 1; ( newtemp = create_node( (strlen(words[i])), words[i] ) ) && i < MAX_NODES; ++i){

    if(newtemp->length == 0){
      break;
    }
    
    temp->next = newtemp;

    if ( newtemp ){
      temp = temp->next;
    }
  }

  return root;
}

void lookup(node * root){
  //Stores search
  char word[100]; 
  char search[5];
  printf("\n\nEnter your word:\n");
  scanf("%s", word);

  //Reduces search characters to 4 letters
  strncpy(search, word, 4);
  
  node * temp;
  int count = 1;

  //Iterates through linked list until a match is found, checks for case sensitivity setting which can be toggled.
  for( temp = root; temp != NULL; temp = temp->next){
    if(case_sensitive == 0){
        if(strcmp(search, temp->lookup)==0){
          printf("\n\nMatch found!: %d %s \n\n", count, temp->word);
          return;
        }
    } else {
        if(strcasecmp(search, temp->lookup)==0){
          printf("\n\nMatch found!: %d %s \n\n", count, temp->word);
          return;
        }
    }
    count ++;
  }

  printf("\n\nNo result found!\n\n");
  
}

void add_word(node * root, char word[100]){
  
  node * temp;
  int i = 0;
  int counter = 0;
  
  //iterates through linked list to find where the new word should be inserted.
  for( temp = root; temp != NULL; temp = temp->next){
    if(strcasecmp(temp->word, word)>0){
      break;
    }
    counter ++;
  }

//Generates a new node for the new word
 node * newtemp = create_node( (strlen(word)), word );
 node * hold;

  //Iterates through linked list until it reaches the correct location, then swaps the current pointer with the new node. On the next iteration, it will set the next pointer for the newly added node unless it needs to be added at the end.
  for( temp = root; temp != NULL; temp = temp->next){
    if(i == counter - 1){
      hold = temp->next;
      temp->next = newtemp;
      i++;
    } else if ( i == counter) {
      temp->next = hold;
      break;
    } else {
      i++;
    }
    if(temp->next == NULL){
      temp->next = newtemp;
    }
  }
  
}

node * destroy_list(node ** root){

  //Unfortuantely not working entirely as inteneded. Ability to replace the dictionary with a new sentence is present, but if the new sentence is shorter in words than the previous it will only overwrite the data as opposed delete it. An attempt is made below to clear the linked list alas it is disfunctional, really missing malloc/free for this task.
  node * temp = *root;
  while(temp != NULL){
    node * hold = temp;
    temp = temp->next;
    hold = NULL;
  }
  *root = NULL;

  node * new_root = create_dictionary();

  return new_root;
}


int main( )
{ 
  node * root = create_dictionary();
  node * temp;
  int running = 1;
  int choice = 0;
  char newWord[100];

  while(running == 1){
    //Prints out the current Dictionary for the user then prompts them to make a selection
    printf("Dictionary:\n");
    printf("=====================\n");
    
    int counter = 1;
    
    for( temp = root; temp != NULL; temp = temp->next){
      printf("%d) %s \n",counter, temp->word);
      counter++;
    }
    printf("=====================\n");
    printf("What would you like to do?:\n\n");
    printf("1. Lookup word\n");
    printf("2. Add word\n");
    printf("3. Switch search mode\n");
    printf("4. delete dictionary and enter new sentence\n");
    printf("5. exit\n----->");
    scanf("%d", &choice);

    switch(choice) {
      case 1 : 
        lookup(root);
      break;

      case 2 :
        printf("\nEnter the word you would like to add:\n");
        scanf("%s", newWord);
        add_word(root, newWord);
        break;
      
      case 3 :
        if(case_sensitive == 1){
            case_sensitive = 0;
            printf("\n\n Case sensitivity has been turned on!\n\n");
        } else {
            case_sensitive = 1;
            printf("\n\n Case sensitivity has been turned off!\n\n");
        }
        break;

      case 4 :
        root = destroy_list(&root);
        break;
        
      case 5 :
        printf("\n\nGoodbye!");
        running = 0;

        break;

      default :
        printf("\n\nUnrecognized option\n\n");
        break;
    }
  }
  
  

    return  0;
}