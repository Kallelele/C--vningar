
/****************************************************************************/
/* DSA node program example  D.F. ROSS                                      */
/****************************************************************************/

/****************************************************************************/
/* include files and  global data objects                                   */
/****************************************************************************/
#include <stdio.h>
#include <stdlib.h>

/****************************************************************************/
/* define types & constants                                                 */
/****************************************************************************/
#define MAXNOD 20
#define INFIN  9999
#define NULLREF NULL

/****************************************************************************/
/* node element definition (this is hidden!)                                */
/****************************************************************************/
typedef struct nodeelem * noderef;

typedef struct nodeelem {
        char       nname;
        int        ninfo;
        noderef    edges;
        noderef    nodes;
        } nodeelem;

/****************************************************************************/
/* define graph instance                                                    */
/****************************************************************************/
static noderef G     = (noderef) NULLREF;       /* adjacency list           */
static int     adjmat[MAXNOD][MAXNOD];          /* adjacency matrix         */

/****************************************************************************/
/* define display structures: Dijkstra-SPT, Floyd, Warshall, Prim           */
/****************************************************************************/
static int  D[MAXNOD];                          /* Dijkstra-SPT distance    */
static char E[MAXNOD];                          /* Dijkstra-SPT edge        */
static int  L[MAXNOD];                          /* Dijkstra-SPT weight      */

static int  Floyd[MAXNOD][MAXNOD];              /* Floyd matrix             */
static int  Warshall[MAXNOD][MAXNOD];           /* Warshall matrix          */

static int  lowcost[MAXNOD];                    /* Prim lowcost             */
static int  closest[MAXNOD];                    /* Prim closest             */

/****************************************************************************/
/* private operations on the node - basic operations                        */
/****************************************************************************/
static int      is_empty(noderef N)             { return N == NULLREF; }

static char     get_nname(noderef N)            { return N->nname; }
static int      get_ninfo(noderef N)            { return N->ninfo; }
static noderef  get_edges(noderef N)            { return N->edges; }
static noderef  get_nodes(noderef N)            { return N->nodes; }

static noderef  set_nname(noderef N, char c)    { N->nname = c; return N; }
static noderef  set_ninfo(noderef N, int v)     { N->ninfo = v; return N; }
static noderef  set_edges(noderef N, noderef E) { N->edges = E; return N; }
static noderef  set_nodes(noderef N, noderef M) { N->nodes = M; return N; }

/****************************************************************************/
/* create and initialise an element in the node                             */
/****************************************************************************/
static noderef create_n(char c, int w)
{
   return set_nodes(
             set_edges(
                set_ninfo(
                   set_nname(malloc(sizeof(nodeelem)), c),
                w),
             NULLREF),
          NULLREF);
}

/****************************************************************************/
/* private operations on the graph - basic operations                       */
/****************************************************************************/
/****************************************************************************/
/* head and tail - a RECURSIVE view of the sequence                         */
/****************************************************************************/
static noderef nhead(noderef N)     { return N; }
static noderef ntail(noderef N)     { return get_nodes(N); }

static noderef ehead(noderef E)     { return E; }
static noderef etail(noderef E)     { return get_edges(E); }

/****************************************************************************/
/* CONStruct a new node with the element at the head of the node            */
/****************************************************************************/
static noderef ncons(noderef e, noderef N) { return set_nodes(e, N); }
static noderef econs(noderef e, noderef E) { return set_edges(e, E); }

/****************************************************************************/
/* display the edges                                                        */
/****************************************************************************/
static void b_edisp(noderef E) {
  if (!is_empty(E)) { 
     printf(" %c(%2d)", get_nname(ehead(E)), get_ninfo(ehead(E)));
     b_edisp(etail(E));
     }
}

/****************************************************************************/
/* display the nodes                                                        */
/****************************************************************************/
static void b_ndisp(noderef G) {
  if (is_empty(G)) printf("\n");
  else { printf(" %c : ", get_nname(nhead(G)));
         b_edisp(get_edges(G)); printf("\n");
         b_ndisp(ntail(G));
         }
}

/****************************************************************************/
/* ADD to the node in ascending order                                       */
/****************************************************************************/
static noderef b_addn(char c, noderef G)
{
	return is_empty(G)			?	create_n(c,0)
	: c < get_nname(nhead(G))	?	ncons(create_n(c,0), G)
	: ncons(nhead(G), b_addn(c, ntail(G)));
}

/****************************************************************************/
/* ADD to the edge in ascending order                                       */
/****************************************************************************/

static noderef b_adde(char c, int w, noderef E)
{
	return is_empty(E) 			? 	create_n(c,w)
	: c < get_nname(ehead(E))	?	econs(create_n(c,w),ehead(E))
	: econs(ehead(E),b_adde(c,w, etail(E)));
}

/****************************************************************************/
/* REMove a  node from the graph                                            */
/****************************************************************************/
static noderef b_remn(char c, noderef G) {
	return is_empty(G)            	? G
    : c == get_nname(nhead(G))		? ntail(G)
    : ncons(nhead(G), b_remn(c,ntail(G)));
}

/****************************************************************************/
/* REMove an edge from the graph                                            */
/****************************************************************************/
static noderef b_reme(char c, noderef E) {
	return is_empty(E) ? E
	: c == get_nname(ehead(E)) ? etail(E)
	: econs(ehead(E), b_reme(c,etail(E)));
}

/****************************************************************************/
/* REMove all edges for a given node from the graph                         */
/****************************************************************************/
static void b_remalle(char c, noderef G) {
	if(!is_empty(G)){
		set_edges(G,b_reme(c,get_edges(G)));
		b_remalle(c,ntail(G));
	}
}

/****************************************************************************/
/* FIND a  node in the graph                                                */
/****************************************************************************/
static noderef b_findn(char c, noderef G) {
	return is_empty(G)				?	NULLREF
	: c == get_nname(nhead(G))		?	nhead(G)
	: b_findn(c, ntail(G));
}

/****************************************************************************/
/* FIND an edge in the graph                                                */
/****************************************************************************/
static noderef b_finde(char c, noderef E) {
	return is_empty(E)				?	E
	: c == get_nname(ehead(E))		?	ehead(E)
	: b_finde(c, etail(E));
}

/****************************************************************************/
/* FIND the number of nodes in the graph (cardinality nodes)                */
/****************************************************************************/
static int b_nsize(noderef G)
{
	return is_empty(G)			?	0
	: 1 + b_nsize(ntail(G));
}

/****************************************************************************/
/* FIND the number of edges in the graph (cardinality edges)                */
/****************************************************************************/
static int b_nedges(noderef E) {
	return is_empty(E)			?	0
	: 1 + b_nedges(etail(E));
  }

static int b_esize(noderef G) {
	return is_empty(G)  ? 0
	: b_nedges(get_edges(G)) + b_esize(ntail(G));
}

/****************************************************************************/
/* CREATE the adjacency matrix (AM)                                         */
/****************************************************************************/
/****************************************************************************/
/* Find the position (index) of a node in the adjacency list (node list)    */
/* NB: this is used to determine the index for the edge                     */
/*     this index is in turn used to fill in the weight in the AM           */
/* e.g. for an adjacency list:                                              */
/*               (position 0)   a => b(3) -> c(2) eol                       */
/*               (position 1)   b => a(3) -> c(7) eol                       */
/*               (position 2)   c => a(2) -> b(7) eol                       */
/*                              eol                                         */
/* get_pos("b") will give 1 (and hence AM[0][1] is set to 3 i.e. a-3-b)     */
/****************************************************************************/
static int get_pos(noderef fnode)  {
	int i=0;
	noderef nnode;
	nnode = G;
	if(!is_empty(fnode)){
		while(get_nname(fnode) > get_nname(nhead(nnode))){
			i++;
			nnode = ntail(nnode);
		}	
	}
	return i;
	/*
	 *is_empty(fnode) || is_empty(G) ? 0 
	 * : get_nname(fnode) == get_nname(G) ? 0
	 * : 1 + get_pos(fnode,ntail(G)); 
	 * */
}

/****************************************************************************/
/* Fill in the values in the adjancy matrix from the adjacency list         */
/* this will give an adjacency matrix:     a       b       c                */
/*                                    -------------------------             */
/*                                    a |  0       3       2                */
/*                                    b |  3       0       7                */
/*                                    c |  2       7       0                */
/****************************************************************************/
static void cre_adjmat() {

   int i,j, nnodes;
   noderef pnode, pedge;

   nnodes = b_nsize(G);
   if (nnodes > MAXNOD) printf("\n *** System failure - too many nodes! ");
   else {
      for (i=0; i<nnodes; i++) for (j=0; j<nnodes; j++) adjmat[i][j] = INFIN;
      pnode = G;
      for (i=0; i<nnodes; i++) {
         pedge = get_edges(pnode);
         while (!is_empty(pedge)) {
            j = get_pos(pedge);
            adjmat[i][j] = get_ninfo(pedge);
            pedge = etail(pedge);
            }
         pnode = ntail(pnode);
         }
     }
}

/****************************************************************************/
/* DISPLAY the adjacency matrix                                             */
/****************************************************************************/

static void b_pline() {
   int i;
   printf("\n ----"); for (i=0; i<b_nsize(G); i++) printf("----");
   }

static void b_mhead(char * header) {

   noderef pnode;

   b_pline(); printf("\n %s", header); b_pline();
   printf("\n     ");
   pnode = G;
   while (!is_empty(pnode)) {
      printf("  %c ", get_nname(pnode));
      pnode = ntail(pnode);
      }
   b_pline();
   }

static void b_mdisp(int m[MAXNOD][MAXNOD], char * header) {

   int i,j, nnodes;
   noderef pnode;

   b_mhead(header);
   nnodes = b_nsize(G); pnode = G;
   for (i=0; i<nnodes; i++) {
      printf("\n "); printf(" %c: ", get_nname(pnode));
      for (j=0; j<nnodes; j++) 
         if (m[i][j] == INFIN) printf("  * ");
         else printf(" %2d ", m[i][j]);
      pnode = ntail(pnode);
      }
   b_pline();
   printf("\n ");
  }

/****************************************************************************/
/* GRAPH ALGORITHMS                                                         */
/****************************************************************************/
/****************************************************************************/
/* Dijkstra-SPT                                                             */
/****************************************************************************/

static void b_dispSPT()  {
	int i;
	for(i=0;i<b_nsize(G);i++){
		//print_headline();
		printf("D: %3d    ",D[i]);
	}
	printf("\n");
	for(i=0;i<b_nsize(G);i++){
		printf("E: %3c    ",E[i]);
	}
	printf("\n");
	for(i=0;i<b_nsize(G);i++){
		printf("L: %3d    ",L[i]);
	}
   //printf("\n TO BE DONE "); 
}
// return the min cost for the edge list
static int min(noderef G,noderef fnode){
	// HITTA POS FÖR NOD W I LISTAN
	noderef snode = G;
	int pos =0;
	while(!is_empty(snode)){
		if(get_nname(fnode) == get_nname(nhead(snode))){
			printf("\nmin pos = %d\n",pos); 
			return pos;
		}else{ pos++;
			snode=ntail(snode);
		}
	}
	return pos;
}

static noderef mincostnode(int w, noderef S,noderef VS){

	noderef node = get_edges(S); 
	noderef sedge;
	noderef fn;
	if(is_empty(node)) return VS;
	while(!is_empty(node)){
	fn = b_findn(get_nname(node),VS);
	printf("%c",get_nname(fn));
	printf("%c",get_nname(node));
	if(get_ninfo(ehead(node)) < w && get_nname(node)==get_nname(fn)){
		w = get_ninfo(ehead(node));
		printf("%d\n",w);
		printf("%c\n",get_nname(ehead(node)));
		printf("knas\n");
		sedge = ehead(node);
		
	}
	node = etail(node);
	}
	return sedge;	
}
/*******************************************************************/
/* FUNKTION FÖR ATT RETURNERA EDGE COST MELLAN w OCH VARJE v i V-S */
/*******************************************************************/
static int edgecost(int v,noderef V,noderef U,noderef S){
	// ska returnera edge cost för w till varje v i V-S
	int i=0;
	noderef edges = NULLREF;
	noderef pedges = U;
	printf("\nV = %c\n",get_nname(V));
	printf("\nU = %c\n",get_nname(U));
	while((i<v) && (!is_empty(pedges))){
		printf("\nEYYO\n");
		  // Användning av b_finde för att kolla om edge finns mellan två noder, om finns ej returnera INFIN annars edge cost
		  pedges = ntail(pedges);
		i++;
	}
	if(!is_empty(pedges)) printf("\npedge = %c\n",get_nname(pedges));
	if(i==v){
		printf("\nWATT\n");
		if(!is_empty(pedges)) edges = b_finde(get_nname(pedges),get_edges(b_findn(get_nname(V),S)));	
		//printf("\nedge = %c\n",get_nname(edges));
		if(is_empty(edges)){
			printf("\nviktorisbad\n");
			return INFIN;
		} else {
			return get_ninfo(edges);
		}
	}
	return INFIN;
}
static int pos_v(noderef VS,int i){
	//int pos;
	if(i!=0 && (!is_empty(nhead(VS)))){
		//pos = get_pos(b_findn(get_nname(nhead(VS)),G));
		printf("\ni = %d\n",i);
		printf("\nnoden = %c\n",get_nname(nhead(VS)));
		return pos_v(ntail(VS),i-1);
		//return pos;
	}else return get_pos(b_findn(get_nname(nhead(VS)),G));
	printf("noden om i = 0 %c\n",get_nname(nhead(VS)));
	printf ("POS OM I = 0 %d\n",get_pos(b_findn(get_nname(nhead(VS)),G)));
	printf("storlek på G = %d\n",b_nsize(G));
	//return get_pos(b_findn(get_nname(nhead(VS)),G));
}
static int choiceofw(int D[],int uvis[]){
	int i,cost,j;
	int wpos=0;
	cost = D[0];
	printf("HEJ\n");
	for(i=0;i<b_nsize(G);i++){
		if(cost>D[i]){
			for(j=0;j<b_nsize(G);j++){
				if(i==uvis[j]) { wpos = i; cost = D[i]; }
			}
			
			
		}
		printf("cost = %d\n",cost);
	}
	return wpos;
}
static char nodetorem(noderef VS, int w){
	noderef node = VS;
	printf("w=%d\n",w);
	printf("nodetorem!\n");
	if(w!=get_pos(b_findn(get_nname(nhead(VS)),G))){
		//printf("Inne i loopen!\n");
		//node = ntail(VS);
		return nodetorem(ntail(VS),w);
	}
	return get_nname(nhead(VS));
}
	
/* V1 AV DIJKSTRA*/

static void b_Dijkstra(char a) {
	noderef S;
	noderef VS = G; // copy nodes to unvisited nodes
	noderef FULL = G;	
	S = b_findn(a,VS); // Visited nodes
	noderef nnode;
	int i;
	int visited[MAXNOD]={0};
	int uvis[MAXNOD];
	noderef sedge = get_edges(S);
	// INITIERING AV ARRAYERNA
	for(i=0;i<=b_nsize(VS);i++){
		if((get_pos(sedge) == i) && !is_empty(sedge)){
		D[i] =  get_ninfo(sedge);// Kostnad mellan start och destinationsmål
		E[i] =  'a';// edge vi går via för att komma fram
		L[i] =  get_ninfo(sedge);// Kostnad för edge mellan två noder
		sedge = etail(sedge);	
		} else {
			D[i] =  INFIN;// Kostnad mellan start och destinationsmål
			E[i] =  'a';// edge vi går via för att komma fram
			L[i] =  INFIN;
		}
	}
	b_dispSPT();
	VS = b_remn(a,VS); // remove start node from unvisited nodes
	for(i=0;i<b_nsize(FULL);i++){
		uvis[i]=i;
	}
	cre_adjmat();
	printf("\n noden som är i V-S: %c\n",get_nname(VS));
	printf("\n noden som är i S: %c\n",get_nname(S));
	printf("\n noden som är i V-S: %d\n",b_nsize(VS));
	printf("\n noden som är i S: %d\n",b_nsize(S));
	//nnode = mincostnode(INFIN,S,VS);
	
	while(!is_empty(VS)){
		//int w = pos_v(nnode,0);
		int w = choiceofw(D,uvis);
		//visited[w]=1;
		uvis[w] = -1;
		char remnode = nodetorem(VS,w);
		// choose w in V-S such that D[w] is a minimum
		printf("\n noden som i V-S: %c\n",get_nname(VS));
		printf("\n noden som i S: %c\n",get_nname(S));
		//S = b_findn(get_nname(nnode),VS); // lägg till node till visited
		//VS = b_remn(get_nname(nnode),VS); // ta bort nod från unvisited
		S = b_findn(remnode,VS); // lägg till node till visited
		nnode = b_findn(remnode,VS);	
		VS = b_remn(get_nname(nnode),VS); // ta bort nod från unvisited
		printf("nod to remove from VS = %c\n",remnode);
		if(!is_empty(VS)) {  
		
		int k;
		printf("\n noden i V-S: %c\n",get_nname(VS));
		for(i=0;i<b_nsize(VS);i++){
			printf("\n v in V-S = %d \n",i);
			printf("\nantal noder i V-S = %d\n",b_nsize(VS));
			printf("\nantal noder i S = %d\n",b_nsize(S));
			printf("\naktuell nod = %c\n",get_nname(nnode));
			printf("\n");
			for(k=0;k<b_nsize(FULL);k++){
				printf( "D[i] = %d ",D[k]);
			}
			for(k=0;k<b_nsize(FULL);k++){
				printf( "uvis[i] = %d ",uvis[k]);
			}
			printf("\n");noderef nodeeller = G;   
			for(k=0;k<b_nsize(G);k++){
				printf("noder i G = %c ",get_nname(nodeeller));
				nodeeller = ntail(nodeeller);
			}
			printf( "G = %d ",b_nsize(G));
			printf("FULL = %d ",b_nsize(FULL));
			//int ecost = edgecost(i,nnode,VS,S);
			printf("D[w] = %d\n",D[w]);
			//printf("\nedge cost = %d\n",ecost);
			int v = pos_v(VS,i);
			printf("v = %d\n",v);
			printf("D[v] = %d\n",D[v]);
			if(/*D[min(G,nnode)]*/D[w] + adjmat[w][v]/*ecost*/ < D[v]) { 
				//printf("\npos = %d\n",min(G,nnode));
				printf("\npos via pos_v%d\n",w);     
				E[v] = get_nname(nnode); //E[v] = w; Edge mellan två relevanta noder get_nname(edge)
				L[v] = adjmat[w][v];//ecost;//get_ninfo(anode); //L[v] = C[w,v]; // lägg till vikten mellan två noder get_ninfo(edge i E)
				D[v] = /*D[min(G,nnode)]*/D[w] + adjmat[w][v];//ecost;//[v] = D[w] + C[w,v];
				b_dispSPT();
			}
		}	
		}
		//if(is_empty(get_edges(nnode))) {VS = b_remn(get_nname(VS),VS);}
		//if(!is_empty(VS)) {nnode = mincostnode(INFIN,S,VS);}
		printf("\nV-S ej tom!\n");
	}		
	b_dispSPT();
	printf("\nV-S = TOM!\n");
}

/****************************************************************************/
/* Floyd                                                                    */
/****************************************************************************/
static void b_Floyd()  {

   printf("\n TO BE DONE ");
   }

/****************************************************************************/
/* Warshall                                                                 */
/****************************************************************************/
static void b_Warshall() {
  
   printf("\n TO BE DONE ");
   }

/****************************************************************************/
/* Prim                                                                     */
/****************************************************************************/

static void b_dispMST()  { 
   
   printf("\n TO BE DONE ");
   }
   
static void b_sumMST() {

   printf("\n TO BE DONE ");
   }

static void b_Prim(char a)     {

   printf("\n TO BE DONE ");
   }                        


/****************************************************************************/
/* public operations on the node                                            */
/****************************************************************************/
void be_display_adjlist()      { b_ndisp(G);}
void be_display_adjmatrix()
   { cre_adjmat(); b_mdisp(adjmat, "Adjacency Matrix"); }

void be_addnode(char c)  { G = b_addn(c, G); }
void be_remnode(char c)  { G = b_remn(c, G); b_remalle(c, G); }

void be_addedge(char cs, char cd, int v) {
   set_edges(b_findn(cs, G), b_adde(cd, v, get_edges(b_findn(cs, G))));
   }

void be_remedge(char cs, char cd) {
   set_edges(b_findn(cs, G), b_reme(cd, get_edges(b_findn(cs, G))));
   }

int be_is_nmember(char c) { return !is_empty(b_findn(c, G)); }

int be_is_emember(char cs, char cd) {
   return be_is_nmember(cs) &&
          !is_empty(b_finde(cd, get_edges(b_findn(cs, G))));
   }

int be_nsize() { return b_nsize(G); }
int be_esize() { return b_esize(G); }

/****************************************************************************/
/* GRAPH ALGORITHMS                                                         */
/****************************************************************************/
void be_Dijkstra(char node)  { b_Dijkstra(node);  }
void be_Floyd()              { b_Floyd();         }
void be_Warshall()           { b_Warshall();      }
void be_Prim(char node)      { b_Prim(node);      }
/****************************************************************************/
/* end of basic functions                                                   */
/****************************************************************************/


