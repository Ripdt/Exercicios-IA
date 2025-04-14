import java.util.LinkedList;

public class BuscadorEmProfundidade extends BuscadorMelhorCaminho {

    public BuscadorEmProfundidade(Mapa_Grid mapa){
    	super(mapa);
    }
    
    protected LinkedList<Nodo> pilha = new LinkedList<>();
    
    public void buscar(int iniX, int iniY, int objX, int objY) {
        caminho = null;
        pilha.clear();
        nodosPercorridos.clear();

        Nodo atual = new Nodo(iniX, iniY);
        pilha.add(atual);

        while (!pilha.isEmpty()) {
            if (atual.x == objX && atual.y == objY) {
                break;
            }

            adicionarNodoPercorrido(atual);

            Nodo[] vizinhos = {
                new Nodo(atual.x, atual.y + 1),
                new Nodo(atual.x + 1, atual.y),
                new Nodo(atual.x, atual.y - 1),
                new Nodo(atual.x - 1, atual.y)
            };

            boolean avancou = false;
            for (Nodo vizinho : vizinhos) {
                if (ehNodoValido(vizinho)) {
                    pilha.add(vizinho);
                    atual = vizinho;
                    avancou = true;
                    break;
                }
            }

            if (!avancou) {
                pilha.removeLast();
                if (!pilha.isEmpty()) {
                    atual = pilha.getLast();
                }
            }
        }
        
        if (atual.x == objX && atual.y == objY) {
            caminho = new int[pilha.size() * 2];
            int i = 0;
            for (Nodo n : pilha) {
                caminho[i++] = n.x;
                caminho[i++] = n.y;
            }
        }
    }
}
