
import java.util.ArrayList;
import java.util.Comparator;

public class BuscadorEmAEstrela extends BuscadorMelhorCaminho {
	public BuscadorEmAEstrela(Mapa_Grid mapa) {
		super(mapa);
	}
	
	private class NodoVisitado extends Nodo {
		public NodoVisitado pai;
		public int f;
		public int g;
		public int h;
		
		public NodoVisitado(int x, int y, NodoVisitado pai) {
			super(x, y);
			
			this.pai = pai;
	        this.g = 0;
	        this.h = 0;
	        this.f = 0;
		}
	}
	
	private class NodoVisitadoComparator implements Comparator<NodoVisitado> {

	    @Override
	    public int compare(NodoVisitado primeiroNodo, NodoVisitado segundoNodo) {
	       return Integer.compare(primeiroNodo.f, segundoNodo.f);
	    }

	}

	private ArrayList<NodoVisitado> listaAberta = new ArrayList<NodoVisitado>();
	
    public void buscar(int iniX, int iniY, int objX, int objY) {
        caminho = null;
        listaAberta.clear();
        nodosPercorridos.clear();

        NodoVisitado atual = new NodoVisitado(iniX, iniY, null);
        listaAberta.add(atual);
        
        while (!listaAberta.isEmpty()) {
        	atual = listaAberta.get(0);
        	listaAberta.remove(atual);
        	adicionarNodoPercorrido(atual);
        	
        	if (atual.x == objX && atual.y == objY) {
        		break;
        	}
        	
        	adicionarNodosVizinhos(atual, objX, objY);
        }
    	
    	if (atual.x == objX && atual.y == objY) {
    		ArrayList<Nodo> caminhoNodos = new ArrayList<Nodo>();
    		do {
    			caminhoNodos.add(atual);
    			atual = atual.pai;
    		} while (atual != null) ;
    		
            caminho = new int[caminhoNodos.size() * 2];
            int i = 0;
            for (Nodo n : caminhoNodos) {
                caminho[i++] = n.x;
                caminho[i++] = n.y;
            }
    	}
    }
    
    private void adicionarNodosVizinhos(NodoVisitado atual, int objX, int objY) {
    	NodoVisitado[] vizinhos = {
            new NodoVisitado(atual.x, atual.y + 1, atual),
            new NodoVisitado(atual.x + 1, atual.y, atual),
            new NodoVisitado(atual.x, atual.y - 1, atual),
            new NodoVisitado(atual.x - 1, atual.y, atual)
        };

        for (NodoVisitado vizinho : vizinhos) {
            if (!ehNodoValido(vizinho)) {
            	continue;
            }
            
        	vizinho.g = vizinho.pai.g + 1;
        	vizinho.h = manhattan(vizinho, objX, objY);
        	vizinho.f = vizinho.g + vizinho.h;
        	
        	NodoVisitado nodoNaLista = buscaNodoNaListaAberta(vizinho);
        	if (nodoNaLista == null) {
                listaAberta.add(vizinho);
        	}
        	else if (nodoNaLista.g > vizinho.g) {
        		listaAberta.remove(nodoNaLista);
                listaAberta.add(vizinho);
        	}
        }
        
        listaAberta.sort(new NodoVisitadoComparator());
    }
    
    private NodoVisitado buscaNodoNaListaAberta(NodoVisitado outro) {
    	for (NodoVisitado nodo : listaAberta) {
    		if (nodo.x == outro.x && nodo.y == outro.y) {
    			return nodo;
    		}
    	}
    	return null;
    } 
    
    private int manhattan(Nodo nodo, int objX, int objY) {
    	return Math.abs(nodo.x - objX) + Math.abs(nodo.y - objY);
    }
}
