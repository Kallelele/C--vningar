
/****************************************************************************/
/* DSA tree program example   D.F. ROSS                                     */
/****************************************************************************/

/****************************************************************************/
/* include files and  global data objects                                   */
/****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/****************************************************************************/
/* define constants & types                                                 */
/****************************************************************************/
#define ARRLEN    100
#define NULLREF   NULL

/****************************************************************************/
/* tree element definition (this is hidden!)                                */
/****************************************************************************/
typedef struct treenode * treeref;

typedef struct treenode {
        int        value;
        treeref    LC;
        treeref    RC;
        } treenode;

/****************************************************************************/
/* define tree instance                                                     */
/****************************************************************************/
static treeref T     = (treeref) NULLREF;

/****************************************************************************/
/* define tree array                                                        */
/****************************************************************************/
static treeref treearray[ARRLEN];

/****************************************************************************/
/* define module variables                                                  */
/****************************************************************************/
static int infomode=0;                          /* demo/info mode           */
static int bstmode =0;                          /* AVL / BST mode           */

/****************************************************************************/
/*  basic operations on the tree                                            */
/****************************************************************************/
static int      is_empty(treeref T)             { return T == NULLREF;     }

static int      get_value(treeref T)            { return T->value;         }
static treeref  get_LC(treeref T)               { return T->LC;            }
static treeref  get_RC(treeref T)               { return T->RC;            }

static treeref  set_value(treeref T,  int v)    { T->value  = v; return T; }
static treeref  set_LC(treeref T, treeref L)    { T->LC     = L; return T; }
static treeref  set_RC(treeref T, treeref R)    { T->RC     = R; return T; }
/****************************************************************************/
/* create and initialise an element in the tree                             */
/****************************************************************************/

static treeref create_node(int v)
{
  return set_RC(
             set_LC(
                   set_value(malloc(sizeof(treenode)), v),
             NULLREF),
          NULLREF);
}

/****************************************************************************/
/* basic operations on the tree                                             */
/****************************************************************************/
/* LC, Node, RC - a RECURSIVE view of the tree                              */
/****************************************************************************/
static treeref node(treeref T)             { return T;         }
static treeref LC(treeref T)               { return get_LC(T); }
static treeref RC(treeref T)               { return get_RC(T); }

/****************************************************************************/
/* FIND the height of the tree                                              */
/****************************************************************************/
static int max(int a, int b) { return a>b ? a : b; }

static int b_height(treeref T)
{
	return is_empty(T) ? 0 
	: 1+max(b_height(LC(T)),b_height(RC(T)));
   //printf("\n TO BE DONE "); 
}
   
/****************************************************************************/
/* Switch between demo and info (stepwise) mode                             */
/****************************************************************************/
static void b_set_run_mode(int m) { infomode = m; }

/****************************************************************************/
/* Switch between AVL and BST mode                                          */
/****************************************************************************/
static void b_set_bst_mode(int m) { bstmode = m;  }

/****************************************************************************/
/* AVL TREE help functions ROTATION FUNCTIONS                               */
/****************************************************************************/
static void b_disp_2D();

static void p_SLR(treeref T) {

  if (!is_empty(T) && infomode) {
     b_disp_2D();
     printf("\n *************************************** ");
     printf("\n *** SLR at node %2d                  *** ", get_value(T));
     printf("\n *************************************** ");
     }
  }
  
static void p_SRR(treeref T) {

  if (!is_empty(T) && infomode) {
     b_disp_2D();
     printf("\n *************************************** ");
     printf("\n *** SRR at node %2d                  *** ", get_value(T));
     printf("\n *************************************** ");
     }
  }
  
static void p_DLR(treeref T) { 

  if (!is_empty(T) && infomode) {
     printf("\n *************************************** ");
     printf("\n *** DLR(T) is SRR(RC(T)) + SLR(T)   *** ");
     printf("\n *************************************** ");
     printf("\n *** DLR at node %2d is               *** ", get_value(T));
     printf("\n *** SRR at node %2d + SLR at node %2d *** ", 
            get_value(RC(T)), get_value(T));
     printf("\n *************************************** ");
     }
  }

static void p_DRR(treeref T) {

  if (!is_empty(T) && infomode) {
     printf("\n *************************************** ");
     printf("\n *** DRR(T) is SLR(LC(T)) + SRR(T)   *** ");
     printf("\n *************************************** ");
     printf("\n *** DRR at node %2d is               *** ", get_value(T));
     printf("\n *** SLR at node %2d + SRR at node %2d *** ",
            get_value(LC(T)), get_value(T));
     printf("\n *************************************** ");
     }
  }

static treeref SLR(treeref T) {
  p_SLR(T);
  
  treeref new = RC(T);
  set_RC(T, LC(new));
  set_LC(new, T);
  T = new;
  return T;
  }

static treeref SRR(treeref T) {
	p_SRR(T);
  
	treeref new = LC(T);
	set_LC(T, RC(new));
	set_RC(new, T);
	T = new;
	return T;
}

static treeref DLR(treeref T) { 
  p_DLR(T);
  
  set_RC(T,SRR(RC(T)));
  T=SLR(T);
  return T;
  }

static treeref DRR(treeref T) {
  p_DRR(T);
  
  set_LC(T,SLR(LC(T)));
  T=SRR(T);
  
  return T;
  }

/****************************************************************************/
/* AVL BALANCE FUNCTION                                                     */
/****************************************************************************/



static int HDiff(treeref T) { 
	
	return is_empty(T)		? 0
	: b_height(LC(T))-b_height(RC(T));
}

static treeref right_rotation(treeref T) {
	if(HDiff(LC(T)) > 0) {

		T = SRR(T);
	}
	else if(HDiff(LC(T)) < 0) {

		T = DRR(T);
	}
	
	return T;
	
}

static treeref left_rotation(treeref T) {
		if(HDiff(RC(T)) > 0) {

		T = DLR(T);
	}
	else if(HDiff(RC(T)) <= 0) {

		T = SLR(T);
	}
	return T;
	
}

static treeref balance(treeref T) {
	
	printf("\n Balance Factor (L-R) at node (%2d):  %2d\n",get_value(T),HDiff(T));
	
	if(HDiff(T) > 1) {
		T = right_rotation(T);
	}
	else if(HDiff(T) <(-1)) {
		T = left_rotation(T);
	}

   return T;
}

/****************************************************************************/
/* CONStruct a new tree from a LC, Node and RC                              */
/*****************************************9 
h=3
bf = -2
10 
h=2
11 
h=1
10 
h=2
bf = 0
9 
h=1
11  
h=1
* ***********************************/
static treeref cons(treeref LC, treeref N, treeref RC) {
   
   set_LC(N, LC); set_RC(N, RC); 
   if(!bstmode) {
	   N = balance(N);
   }
	   return N;
}

/****************************************************************************/
/* ADD to the tree in AVL - BST order + balance                             */
/****************************************************************************/
static treeref b_add(treeref T, int v)
{
	
  return is_empty(T)         ? create_node(v)
    : v < get_value(node(T)) ? cons(b_add(LC(T), v), node(T), RC(T))
    : v > get_value(node(T)) ? cons(LC(T), node(T), b_add(RC(T), v))
    :                          T;
}

/****************************************************************************/
/* REMove an element from the tree / BST order                              */
/****************************************************************************/

static treeref max_value(treeref T)
{
	return 	is_empty(T)		?	0
	:		is_empty(RC(T))	?	T
	:		max_value(RC(T));
}

static treeref min_value(treeref T)
{
	return 	is_empty(T)		?	0
	:		is_empty(LC(T))	?	T
	:		min_value(LC(T));
}

static treeref b_rem(treeref T, int v);

static treeref b_rem_2children(treeref T) {
	if(HDiff(T)>0){
		treeref L = max_value(LC(T));
		return cons(b_rem(LC(T),get_value(L)),L,RC(T));
	}
	else {
		treeref R = min_value(RC(T));
		return cons(LC(T),R,b_rem(RC(T),get_value(R)));
	}
// Annan version 	
treeref L = max_value(LC(T));
treeref R = min_value(RC(T));
return HDiff(T) > 0  ?  cons(b_rem(LC(T),get_value(L)),L,RC(T)) 
    : cons(LC(T),R,b_rem(RC(T),get_value(R)));
}





static treeref rem_root(treeref T)
{
	return is_empty(LC(T)) && is_empty(RC(T)) 	? 0 
	: is_empty(LC(T)) 							? RC(T)
	: is_empty(RC(T)) 							? LC(T)
	: b_rem_2children(T);
}

static treeref b_rem(treeref T, int v)
{
	 return is_empty(T)		        ?  	T
	 : v< get_value(node(T))		? 	cons(b_rem(LC(T), v), node(T), RC(T))
	 : v> get_value(node(T))		? 	cons(LC(T),node(T),b_rem(RC(T),v))
	 : rem_root(T);
}

/****************************************************************************/
/* FIND an element in the BST (Binary Search Tree)                          */
/****************************************************************************/
static int b_findb(treeref T, int v)
{
	return is_empty(T) ? 0
	: v<get_value(node(T)) ?  	b_findb(LC(T),v)
	: v>get_value(node(T)) ? 	b_findb(RC(T),v)
	: 1;
}

/****************************************************************************/
/* FIND the number of elements in the tree (cardinality)                    */
/****************************************************************************/

static int b_size(treeref T)
{
	return is_empty(T) ? 0 : 1 +( b_size(LC(T))+b_size(RC(T)));
   }

/****************************************************************************/
/* display the tree ELEMENT                                                 */
/****************************************************************************/
static void b_disp_el(treeref T)
{
   if (!is_empty(T)) printf(" %2d ", get_value(node(T)));
   else printf("  * ");
}

/****************************************************************************/
/* display the tree (pre-order)                                             */
/****************************************************************************/
static void b_disp_pre(treeref T)
{
	if(!is_empty(T)){
		b_disp_el(node(T));
		b_disp_pre(LC(T));
		b_disp_pre(RC(T));
	}
}
static void b_print_tree(treeref T)
{
   if (!is_empty(T)) printf("%5d   ", get_value(node(T)));
   else printf("    *   ");
}
/****************************************************************************/
/* display the tree (in-order)                                              */
/****************************************************************************/
static void b_disp_in(treeref T)
{
	if (!is_empty(T)) {
		b_disp_in(LC(T));
		b_disp_el(node(T));
		b_disp_in(RC(T));
    }
}
/****************************************************************************/
/* display the tree (post-order)                                            */
/****************************************************************************/
static void b_disp_post(treeref T)
{
	 if (!is_empty(T)) {
		 b_disp_post(LC(T));
		 b_disp_post(RC(T));
		 b_disp_el(node(T));
	 }
}

/****************************************************************************/
/* display the treearray array                                              */
/****************************************************************************/
static void b_disp_array()
{
	int i;
	if(!is_empty(T)){
		for(i=0;i<(pow(2,b_height(T)))-1;i++){
			b_disp_el(treearray[i]);
		}	
	}
}

/****************************************************************************/
/* display the TREE information                                             */
/****************************************************************************/
static void T2Q(treeref T, int qpos);
static void print_menuline(){
	int i;
	if(b_height(T)>=4){
	for(i=0;i<pow(2,b_height(T)-1);i++){
		printf("----");
	}	
	}
}
static void print_treeinfo() {
   printf("\n ---------------------------------------");print_menuline();
   printf("\n TREE:        nodes: %2d   height: %d ", b_size(T), b_height(T));
   printf("\n ---------------------------------------");print_menuline();
   printf("\n inorder:   "); b_disp_in(T);
   printf("\n preorder:  "); b_disp_pre(T);
   printf("\n postorder: "); b_disp_post(T);
   printf("\n Treearray: "); b_disp_array();
   printf("\n ---------------------------------------");print_menuline();
   printf("\n");
}

/****************************************************************************/
/* Tree to array via a treearray (breadth-first search)                     */
/****************************************************************************/
/* Transforms a binary tree to a sequence (including NULL (*) references)   */
/* e.g.  the following tree:                    5                           */
/*                                      2              7                    */
/*                                  *       3      6        *               */
/*                                                                          */
/* becomes: [5] [2] [7] [*] [3] [6] [*]                                     */
/****************************************************************************/
static void T2Q(treeref T, int qpos) {
	treearray[qpos]=T;
	if(!is_empty(T) && qpos<ARRLEN){		
		T2Q(LC(T),(qpos*2)+1);
		T2Q(RC(T),(qpos*2+1)+1);
	} 
}

/****************************************************************************/
/* display the tree in 2D                                                   */
/****************************************************************************/
/* step 1: transform the tree to an array (Q) using T2Q() above             */
/* e.g. array [5] | [2] [7] | [*] [3] [6] [*]   | etc.                      */
/*      index (1) | (2) (3) | (4) (5) (6) (7)   | (8) ...                   */
/*      level (1) | (2) (2) | (3) (3) (3) (3)   | (4) ...                   */
/* step 2: then print the nodes at each level of the tree to give           */
/*                             5                                            */
/*                     2              7                                     */
/*                *        3      6        *                                */
/****************************************************************************/

void print_space(int height)
{
	int i, bredd;
	bredd = pow(2,b_height(T))/pow(2,height);
	if(height<b_height(T)){
		for(i=1;i<bredd;i++){
			printf("    ");
		}
	}
}

static void disp_tree(int height)
{
	int i;
	if(height<=b_height(T)){
		for(i=pow(2,height-1);i<=pow(2,height)-1;i++){
			print_space(height);
			b_print_tree(treearray[i-1]);
			print_space(height);
		}
		if(height<b_height(T)){printf("\n");}
		disp_tree(height+1);
	}
	
}

static void b_disp_2D()
{
   if (!is_empty(T)) {
	  printf("\n");
	  T2Q(node(T),0);
	  disp_tree(1);
      if (infomode) {
		  print_treeinfo();
	  }
       
   }
   else printf("\n *** Tree is empty \n");
   }

/****************************************************************************/
/* public operations on the tree                                            */
/****************************************************************************/
void be_disp_2D()                { b_disp_2D();          }
void be_add(int v)               { T = b_add(T, v);      }
void be_rem(int v)               { T = b_rem(T, v);      }
int  be_is_memberb(int v)        { return b_findb(T, v); }
void be_set_run_mode(int v)      { b_set_run_mode(v);    }
void be_set_bst_mode(int v)      { b_set_bst_mode(v);    }
/****************************************************************************/
/* end of basic functions                                                   */
/****************************************************************************/



