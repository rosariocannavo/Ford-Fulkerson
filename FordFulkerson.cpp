#include <iostream>
#include <climits>
#include <queue>
#define White 0
#define Black 2
using namespace std;

template<class H> class MGraph{
    private:
        int maxLen;                                         //lunghezza massima del grafo, 
        int nodeNumber;                                     //numero di nodi e 
        int edgeNumber;                                     //numero di archi
        H **Key;                                            //vettore delle chiavi
        int **AdjM;                                         //matrice di adiacenza

        int findIndex(H x){                                 //dato in input un valore torna l'indice a cui è situato nel vettore delle etichette
            for(int i=0;i<nodeNumber;i++){
                if(*Key[i]  == x) return i;                 //scorre fino a quando non trova la chiave, altrimenti torna -1
            }
            return -1;
        }

    public:
        MGraph(int maxLen){                                 //costruttore
            this->maxLen = maxLen;                          //imposta la dimensione massima del grafo
            nodeNumber=edgeNumber=0;                        //inizializza a 0 numero di archi e nodi
            Key = new H*[maxLen];                           //inizializza il nodo delle etichette
            for(int i=0; i<maxLen;i++) Key[i] = NULL;       //imposta tutte le etichette a null 
            AdjM = new int*[maxLen];                        //inizializza la matrice di adiacenza tutta a 0
            for(int i=0; i<maxLen;i++){
                AdjM[i] = new int[maxLen];                  
                for(int j=0;j<maxLen;j++){
                    AdjM[i][j] = 0;
                }
            }                                         
        }


        int getMaxNodeNumber(){                              //ritorna la dimensione massima del grafo
		    return maxLen;
	    }


        int getCurrentNodeNumber(){                          //ritorna il numero di nodi presenti attualmente all'interno del grafo
		    return nodeNumber;
	    }


	    int getCurrentEdgeNumber(){                          //ritorna il numero di archi presenti  attualmente all'interno del grafo
		    return edgeNumber;
	    }


        int** getGraphMatrix(){                              //ritorna la matrice di adiacenza
		    return AdjM;
	    }


        H* getKey(int index){                                //ritorna in valore di una chiave dato l'indice
            if(index < 0 || index >= nodeNumber)    
                return NULL;
            return Key[index];
	    }


        MGraph<H>* addNode(H k){                                         //aggiunge un nodo
            if(nodeNumber==maxLen)  return this;                         //se il grafo è pieno non posso aggiungere altri elementi
            if(findIndex(k)>=0) return this;                             //se l'elemento è già presente non posso aggiungerlo nuovamente
            Key[nodeNumber] = new H(k);                                  //istanzio un nuovo elemento nel vettore delle etichette
            nodeNumber++;                                                //incremento il numero di nodi presenti nel grafo
            return this;                                                 //ritorno il puntatore per concatenare le operazioni
        }


        MGraph<H>* addEdge(H x, H y, int peso){                          //aggiunge un arco
            int i = findIndex(x);                                        //cerca l'indice del primo nodo in Key
            int j = findIndex(y);                                        //cerca l'indice del secondo nodo in Key
            if(i < 0 || j < 0) return this;                              //se uno dei due nodi non esiste l'arco non può essere creato
            if(AdjM[i][j] == 0){                                         //se l'arco è gia presente nella matrice non posso inserirne uno uguale
                AdjM[i][j] = peso;                                       //inserisce l'arco specificandone il relativo peso
                edgeNumber++;                                            //incrementa il numero di archi del grafo
            }
            return this;
        }


        bool bfs(int** rGraph, int s, int t, int parent[]){              //funzione bfs per calcolare i cammini
            queue <int> Q;                                               //istanzia una coda dalla STL
            int color[nodeNumber];                                       //array dei colori, (indica il colore di ogni nodo)
            for(int i=0;i<nodeNumber;i++){                               //inizializza tutti i nodi a bianco (non visitato)
                color[i] = White;                                        //e inizializza il vettore dei predecessori a -1
                parent[i]=-1;                   
            }
            Q.push(s);                                                   //inserisce nella coda l'elemento da cui partire per la visita
            while(!Q.empty()){                                           //fino a quando esistono nodi da visitare
                int u = Q.front();                                       //visualizza il primo elemento dalla coda
                Q.pop();                                                 //rimuove il primo elemento della coda       
                for(int v=0; v<nodeNumber; v++){                         //per ogni nodo adiacente al nodo preso in esame, se non visitato 
                    if( (rGraph[u][v] > 0) && (color[v] == White) ){     //imposta il colore a nero (visitato)
                            color[v] = Black;                            //inserisce l'elemento nella coda per visitarlo successivamente                           
                            Q.push(v);  
                            parent[v] = u;
                    }
                }
            }                                                            //se il pozzo è visitato da un percorso durante la bfs ritorna true
            return(color[t] == Black);                                   //altrimenti non esiste nessun percorso dalla sorgente al pozzo passsando per rGraph (ritorna false)
        }


        int fordFulkerson(int s, int t){                                //complessità: O(E |f∗|)  dove f∗ è il flusso massimo trovato dall’algoritmo
            if(nodeNumber == 0)  return -1;                             //controlla il caso limite (grafo vuoto)
            int u, v; 
            int** rGraph;                                               //crea un grafo residuo e lo riempie con le capacità residue date dal grafo originale
            rGraph = new int*[nodeNumber];
            for(u = 0; u < nodeNumber; u++)
                rGraph[u] = new int[nodeNumber];
            
            for(u = 0; u < nodeNumber; u++) 
                for(v = 0; v < nodeNumber; v++) 
                    rGraph[u][v] = AdjM[u][v]; 

            int parent[nodeNumber];                                     //l'array parent sarà riempito dalla BFS per memorizzare il percorso
            int max_flow = 0;                                           // inizializza il flusso massimo a 0                      

            while(bfs(rGraph, s, t, parent)){                           //aumenta il flusso fino a quando esiste un percorso da s a t
                int path_flow = INT_MAX;                                //cerca la capacità residua minima degli archi sut tutto il percorso creato dalla BFS, tramite questo troviamo il flusso massimo 
                for(v=t; v!=s; v=parent[v]){ 
                    u = parent[v]; 
                    path_flow = min(path_flow, rGraph[u][v]); 
                }                 
                for(v=t; v != s; v=parent[v]){                          //aggiorna le capacità residue dei vertici e dei vertici opposti lungo il percorso 
                    u = parent[v]; 
                    rGraph[u][v] -= path_flow; 
                    rGraph[v][u] += path_flow; 
                } 
                max_flow += path_flow;                                  //aggiunge il flusso del percorso al flusso massimo 
            } 
            return max_flow;                                            //ritorna un intero che rappresenta il flusso massimo ottenibile tra il pozzo e la sorgente
        } 


        void print(){                                                   //stampa i nodi adiacenti
            for(int i=0;i<nodeNumber;i++){
                if(i == 0) cout<<"(S; "<<"S"<< ") = ";                  //identifico la sorgente e ne stampo i nodi adiacenti
                else if(i == maxLen-1) continue;                        //non stampo la lista di adiacenza del pozzo perchè non ha mai archi uscenti  
                else cout<<"("<<i<<"; "<<*Key[i]<< ") = ";
                for(int j=0;j<nodeNumber;j++){
                    if(AdjM[i][j]) 
                        cout<<"  {"<<*Key[j]<<", "<<AdjM[i][j]<<"}; ";
                }
                cout<<endl;
            }
        }

        void printMatrix(){                                             //stampa la matrice di adiacenza
            if(nodeNumber == 0){
                cout<<"Grafo vuoto"<<endl;                              //se il grafo è vuoto non effettua la procedura
                return;
            } 
            cout<<"Adjacency matrix"<<endl;
            cout<<"   ";
            for(int a=0;a<nodeNumber;a++)    cout<<Key[a]<<":   ";
            cout<<endl;
            for(int i=0;i<nodeNumber;i++){
                cout<<Key[i]<<":  ";
                for(int j=0;j<nodeNumber;j++){
                    cout<<AdjM[i][j]<<"    ";
                }
                cout<<endl;
            }
            cout<<endl;
        }
};

int main(){
    //TEST 1
    cout<<endl<<"GRAPH 1"<<endl<<endl;                                                          //primo grafo di test
    MGraph<char> *Gr = new MGraph<char>(6);                                                     //alloca un nuovo grafo con dimensione massima 6
	Gr->addNode('0')->addNode('1')->addNode('2')->addNode('3');                                 //aggiunge tutti i nodi specificandone il nome
	Gr->addNode('4')->addNode('5');
	Gr->addEdge('0','1',16)->addEdge('0','2',13);                                               //aggiunge tutti gli archi con relativi pesi
	Gr->addEdge('1','2',10)->addEdge('1','3',12);
	Gr->addEdge('2','1',4)->addEdge('2','4',14);
	Gr->addEdge('3','5',20)->addEdge('3','2',9);
	Gr->addEdge('4','3',7)->addEdge('4','5',4);
    Gr->print();                                                                                //stampa tutti i nodi e i rispettivi nodi adiacenti
    cout<<endl;
    Gr->printMatrix();                                                                          //stampa la matrice di adiacenza
    cout<<"Maximum Flow: "<<Gr->fordFulkerson(0,5);                                             //invoca la procedura di fordFulkerson passando sorgente e pozzo
    cout<<endl;

    //TEST 2
    cout<<endl<<"GRAPH 2"<<endl<<endl;                                                          //secondo grafo di test
    MGraph<char> *Gr2 = new MGraph<char>(6);                                                    //alloca un nuovo grafo con dimensione massima 6
    Gr2->addNode('0')->addNode('1')->addNode('2')->addNode('3')->addNode('4')->addNode('5');    //aggiunge tutti i nodi specificandone il nome
    Gr2->addEdge('0','1',7)->addEdge('0','3',10)->addEdge('0','2',8);                           //aggiunte tutti gli archi con relativi pesi            
    Gr2->addEdge('1','5',21)->addEdge('1','4',1);
    Gr2->addEdge('2','4',12);
	Gr2->addEdge('3','2',2)->addEdge('3','4',5)->addEdge('3','1',11);                           
	Gr2->addEdge('4','5',14);
    Gr2->print();                                                                               //stampa tutti i nodi e i rispettivi nodi adiacenti
    cout<<endl;
    Gr2->printMatrix();                                                                         //stampa la matrice di adiacenza
    cout<<"Maximum Flow: "<<Gr2->fordFulkerson(0,5);                                            //invoca la procedura di fordFulkerson passando sorgente e pozzo
    cout<<endl;

    //TEST 3
    cout<<endl<<"GRAPH 3"<<endl<<endl;                                                          //terzo grafo di test
    MGraph<char> *Gr3 = new MGraph<char>(5);                                                    //alloca un nuovo grafo con dimensione massima 5
    Gr2->addNode('0')->addNode('1')->addNode('2')->addNode('3')->addNode('4');                  //aggiunge tutti i nodi specificandone il nome
    Gr2->addEdge('0','1',100)->addEdge('0','2',100);                                            //aggiunge tutti gli archi con relativi pesi
    Gr2->addEdge('1','3',7);    
    Gr2->addEdge('2','3',3);
	Gr2->addEdge('3','4',1)->addEdge('3','4',5)->addEdge('3','1',11);
	Gr2->addEdge('4','5',14);
    Gr2->print();                                                                               //stampa tutti i nodi e i rispettivi nodi adiacenti
    cout<<endl;                     
    Gr2->printMatrix();                                                                         //stampa la matrice di adiacenza
    cout<<"Maximum Flow: "<<Gr2->fordFulkerson(0,4);                                            //invoca la procedura di fordFulkerson passando sorgente e pozzo
    cout<<endl;

    //TEST 4    
    cout<<endl<<"GRAPH 4"<<endl<<endl;                                                          //quarto grafo di test
    MGraph<char> *Gr4 = new MGraph<char>(4);                                                    //alloca un nuovo grafo con dimensione massima di 4
    Gr4->addNode('0')->addNode('1')->addNode('2')->addNode('3');                                //aggiunge tutti i nodi specificandone il nome
    Gr4->addEdge('0','1',3)->addEdge('0','2',2);                                                //aggiunge tutti gli archi con relativi pesi
    Gr4->addEdge('1','3',2)->addEdge('1','2',5);
    Gr4->addEdge('2','3',3);
    Gr4->print();                                                                               //stampa tutti i nodi e i rispettivi nodi adiacenti
    cout<<endl;
    Gr4->printMatrix();                                                                         //stampa la matrice di adiacenza
    cout<<"Maximum Flow: "<<Gr4->fordFulkerson(0,3);                                            //invoca la procedura di fordFulkerson passando sorgente e pozzo
    cout<<endl;

    //TEST 5 (caso limite)                                                                      
    cout<<endl<<"GRAPH 5"<<endl<<endl;                                                          //quinto grafo di test (caso limite: grafo sprovvisto di nodi)
    MGraph<char> *Gr5 = new MGraph<char>(5);                                                    //alloco un grafo con dimensione massima di 5
    Gr5->print();                                                                               //stampo tutti i nodi e i rispettivi nodi adiacenti
    cout<<endl;
    Gr5->printMatrix();                                                                         //stampo la matrice di adiacenza
    cout<<"Maximum Flow: "<<Gr5->fordFulkerson(0,4);                                            //invoca la procedura di fordFulkerson passando sorgente e pozzo    
    cout<<endl;
    
    return 0;
}