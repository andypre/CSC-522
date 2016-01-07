//Navpreet Kaur
//The objective of this program is to find dominating set for a graph. This program is based on heuristics concept.
//A part of Tabu list heuristic is used .There are two array in this program dominated[MMAX] and deleted[MMAX] which keep track of which vertices 
//dominated and which vertices are exculded from red vertices. So, a part of tabu list is used. Once a inital solution is genearted an evaluation function works 
// for number times a graph has vertices.Each time Evalution function check if current vertex have num_dominated value i.e number of times a vertex is dominated.
//If num_Dominated is greater than one that vertex is selected for making improvements in current solution. If it leads to minimum dominating set that vertex 
//is selected as dominating vertex.

#define MAIN 1
#include "headerfile.h"
#define DEBUG 0
using namespace std;
int main()
{
	int n,m,min_size,graph_number=0; //min_size variable is used to store minimum size of dominating set
	int G[NMAX][MMAX]; //G is array used to store read graph
	int dominated[MMAX]; //dominated is array which keep track of elements that are dominated.
	int dom_set[MMAX]; //dom_set is array used to store dominating vertices
	int min_dom_set[MMAX]; //min_dom_set store the minimum dominating set found so far
	int num_dominated[NMAX]; //num_dominated array store how many times a vertex is dominated
	while(read_graph(&n,&m,G))
	{
	   graph_number++;
	   printf("\n %3d %3d \n",graph_number,n);
	   find_dom_set(n,m,G,dominated,dom_set,min_dom_set,num_dominated,&min_size); //This function returns the minimum dominating set value
	   print_ver(1, &min_size,n,min_dom_set);
	}

}
int read_graph(int *n, int *m, int G[NMAX][MMAX]) //read_graph reads the input graph
{
 	int i, j, u, d;

    if (scanf("%d", n)!= 1) return(0);
     if(*n > NMAX)  //If enetered graph have number of vertices greater than NMAX. Message will be displayed on screen
     {
     	printf("\n");
     	printf("Increase NMAX and recomplie");
     	return(0);
     }
    *m= (*n+31)/32;


    for (i=0; i < *n; i++)
    {
        for (j=0; j < *m; j++)
        {
            G[i][j]= 0; //  Initialize the graph to have no edges.

        }
    }


    for (i=0; i < *n; i++)  //  Read in the graph.

    {
        if (scanf("%d", &d)!=1) return(0);
        for (j=0; j < d; j++)
        {
            if (scanf("%d", &u)!=1) return(0);
            ADD_ELEMENT(G[i], u);
            ADD_ELEMENT(G[u], i);
        }
    }
    return(1);
}
int find_dom_set(int n,int m,int G[NMAX][MMAX],int dominated[MMAX],int dom_set[MMAX],int min_dom_set[MMAX],int num_dominated[NMAX],int *min_size)
{
	int u=0;
	//random_dom_set function selects the initial solution of dominating set for graph. The logic of this function is based on Tabu list logic.
	random_dom_set(n,m,G,dominated,dom_set,min_dom_set,num_dominated,min_size); 
	
	//After a random intial solution is generated. A evaluation function is set.This evaluation function works based on number of vetrtices of graph.
	//The better_dom function works to find better solution for dominating by making changes in current generated solution.
	//The changes are based on value of num_dominated array.If a vertex have num_dominated value more than once.Which means that vertex is dominated by 
	//more than one red vertex.So by making that vertex red by excluding already red neigbhbor vertices if a better dominating set found that dominating set 
	//is considered the minimum dominating set.   
	while(u<n)
	{
		better_dom(n,m,u,G,dominated,dom_set,min_dom_set,num_dominated,min_size);
		 u++;
	}
	
}
//Random function generates the random dominating set  solution for graph.
int random_dom_set(int n,int m,int G[NMAX][MMAX],int dominated[MMAX],int dom_set[MMAX],int min_dom_set[MMAX],int num_dominated[NMAX],int *min_size)
{
	int i,j,u,size=0,d_size;
	for(i=0;i<m;i++)   //initialising array.
	{
		dominated[i]=0;
		dom_set[i]=0;
		min_dom_set[i]=0;
		
	}
	
	for (i=0; i < n; i++)
		ADD_ELEMENT(G[i], i); //setting diagnal elements of array
	for(i=0;i<n;i++)
		num_dominated[i]=0; //intialising num_dominated array to have no value
	for(i=0;i<n;i++)
	{
		u=i;           //u is the vertex.
		
		//Before processing any vertex first it checks if that vertex is alredy in dominated set.IF yes that means that vertex is already traversed.So we can not make that vertex red.
		if(!IS_ELEMENT(dominated,u))  
		{
			ADD_ELEMENT(dom_set,u); //Read u and add it to dom_set.Here u is red vertex.
			size++;                 //increment size for partial dominating set
			for(j=0;j<m;j++)
			dominated[j] |= G[u][j];  //record all the neighbours v and of vertex u recorded in G. 
			for(j=0;j<n;j++)
			if(IS_ELEMENT(G[u],j))    //increment the value of num_dominated for all the neighbours v of vertex u. 
			num_dominated[j]=num_dominated[j]+1;
			
			#if DEBUG
			printf("\n VERTEX IS %d",u);
			printf("\n DOM_SET :");
			print_set(n,dom_set);
			printf("\n DOMINATED");
			print_set(n,dominated);
			printf("\n NUM_DOMINATED ");
			for(j=0;j<n;j++)
			printf(" %d",num_dominated[j]);
			#endif
		}	
	    
	}
	   	for(j=0;j<m;j++)
	   		min_dom_set[j]=dom_set[j]; //coppy the dominating set into min_dom_set.
	    *min_size=size;           //set the size of min_size
	    
       print_ver(0, min_size,n,min_dom_set);  //function for print dominating set vertices
	 	#if DEBUG
		printf("MIN_DOM_SET :");
		print_set(n,min_dom_set);
		printf("MIN_SIZE %d",*min_size);
		#endif
}

//if a vertex have num_dominated > 1 that means that vertex can be a good choice for being red.So make that vertex u red and delete the red neighbours of
//that vertex and check if the graph is still dominated.If graph is dominated that means the u is good choice for red vertex.
int better_dom(int n,int m,int u,int G[NMAX][MMAX],int dominated[MMAX],int dom_set[MMAX],int min_dom_set[MMAX],int num_dominated[NMAX],int *min_size)
{
	 int i,j,n_choice=0,choice=0,size_del=0,size_dom,n_domi=0,store;
	 int deleted[MMAX];  //this array keep the track of deleted vertices.
   
     for(j=0;j<m;j++)
    	 deleted[j]=0; //intialising deleted array so that it have no value
     
	if(num_dominated[u]>1)
	{
	  	
		for(i=0;i<n;i++)
		if(IS_ELEMENT(G[u],i))
		{
			num_dominated[i]=num_dominated[i]+1;  //if a vertex u have num_dominated value greater than 1. Increase the num_dominated for all the neighbours V of vertex u as recorded in G.
		}
		
		#if DEBUG
	    printf("\n CHOICE OF VERTEX %d",u);
		printf("\n NUM_DOMINATED :");
		for(j=0;j<n;j++)
		printf("%d",num_dominated[j]);
		#endif
		
		for(i=0;i<n;i++)
		{
			if(IS_ELEMENT(G[u],i) && i!=u)   //checking neigbhbours of vertex u as recorded in G.
			{
				if(IS_ELEMENT(dom_set,i))  //checking for neighbours of vertex u which are in dom_set i.e, already red vertices.
				{
					
					#if DEBUG
					printf("\n choosed dom_Set %d",i);
					printf("\n NEIGHBOURS OF CHOOSED VERTEX IS %d:",i);
					#endif
				
					n_choice=0;       
					for(j=0;j<n;j++)
					if(IS_ELEMENT(G[i],j))
					num_dominated[j]=num_dominated[j]-1;     //decrease num_dominated for all the red neighbours v of vertex u. 
					for(j=0;j<n;j++)
					if(num_dominated[j]==0)
					n_choice=1;                   //check if num_dominated voilates. i.e, is num_dominated is 0 which means that vertex v can not be deleted.
					
					if(n_choice==1)
					{
						
						for(j=0;j<n;j++)
						if(IS_ELEMENT(G[i],j))
				    	num_dominated[j]=num_dominated[j]+1;  //increase num_dominated for vertex v which voilated num_dominated.
						n_choice==0;
						#if DEBUG
						printf("\n NUM_DOMINATED VOILATES %d",i);
						printf("\n NUM_DOMINATED :");
						for(j=0;j<n;j++)
						printf("%d",num_dominated[j]);
						#endif
						
					
					}
					else
					{
						#if DEBUG
						printf("\n NUM_DOMINATED :");
						for(j=0;j<n;j++)
						printf("%d",num_dominated[j]);
						#endif
						
					    ADD_ELEMENT(deleted,i); //If num_dominated not voilated which means v is good choice so delete v and add it to deleted array.which keep track of deleted vertices.
						DEL_ELEMENT(dom_set,i);
						
						#if DEBUG
						printf("\n ADDED TO DELETED:");
						print_set(n,deleted);
						printf("\n DELETED FROM DOM_SET");
						print_set(n,dom_set);
						#endif
					}
				}
			}
		}
	
		size_del=set_size(n,deleted);   //check size of deleted array.
		if(size_del==1)               //if only one vertex is deleted.Which means not a good choice because after deleting one  and adding one does not find minimum dominating set. 
		{
		    for(j=0;j<n;j++)
		    if(IS_ELEMENT(deleted,j))   //check for vertex which is deleted.
		   	{
		   		store=j;
		   		break;
		   	}
		   	DEL_ELEMENT(deleted,store);  //delete that cetex form deleted array
		   	
		   	ADD_ELEMENT(dom_set,store);  //add deleted vertex again to dom_set
		   	#if DEBUG
		    printf("RESTORE %d",store);
	        printf("\n ADDED TO DELETED:");
		    print_set(n,deleted);
		    printf("\n DELETED FROM DOM_SET");
		    print_set(n,dom_set);
		   	#endif
		   	for(j=0;j<n;j++)
			   	if(IS_ELEMENT(G[store],j))
			   		num_dominated[j]=num_dominated[j]+1;     //increase num_dominated accordingly.
		   	
			for(j=0;j<n;j++)
				if(IS_ELEMENT(G[u],j))
				{
					num_dominated[j]=num_dominated[j]-1;   //decrease num_dominated for selected u vetex which was used to find good solution.
				}
	
		}
		else if(size_del>1)     //if size of deleted array is greater than one which means a good choice. 
		{
			ADD_ELEMENT(dom_set,u); //Add u to dom_set
		
		}
		else if(size_del==0)     //if no vetex is deleted i.e, u is not good choice
		{
		    for(j=0;j<n;j++)
			if(IS_ELEMENT(G[u],j))
			{
				num_dominated[j]=num_dominated[j]-1; //decrease num_dominated for selected u vertex.
			}	
		}
		
		for(j=0;j<n;j++)
			if(num_dominated[j]==0)     //check for num_dominated
				n_domi=1;	
	
		#if DEBUG
		printf("NUM_DOMINATED :");
		for(j=0;j<n;j++)
		printf("%d",num_dominated[j]);
		#endif
		
		
		size_dom=set_size(n,dom_set);    //check size of dom_set
		if(size_dom<*min_size)         //if size is better than min_sze so far
		{
			if(n_domi==0)             //if all the vertices are dominated
			{
				
				for(j=0;j<m;j++)
					min_dom_set[j]=dom_set[j];   //copy dom_set to min_dom_set
				*min_size=size_dom;         //update min_size
				print_ver(0, min_size,n,min_dom_set); //print the current dominating set vertices.
			}
		}
	}
}
//set size used to compute the size of array
int set_size(int n, int set[])
{
    int j, m, d;

    m= (n+31)/ 32;

    d=0;

    for (j=0; j < m; j++)
    {
       d+= POP_COUNT(set[j]);
    }
    return(d);
}
// Prints a set.
void print_set(int n, int set[])
{
   int i;

   for (i=0; i < n; i++)
       if (IS_ELEMENT(set, i))
           printf(" %3d", i);
           fflush(stdout);
   		   printf("\n");
}
//print the domiating set vertices
int print_ver(int status, int *min_size,int n, int min_dom_set[])
{  
    
    printf("%1d %3d ", status, *min_size);
    print_set(n,min_dom_set);
    fflush(stdout);
}
