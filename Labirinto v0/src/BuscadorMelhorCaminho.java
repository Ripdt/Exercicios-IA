import java.util.*;
import java.util.concurrent.ConcurrentHashMap;

public abstract class BuscadorMelhorCaminho {

    protected Set<Integer> nodosPercorridos = ConcurrentHashMap.newKeySet();
    protected int[] caminho;
    protected Mapa_Grid mapa;

    protected BuscadorMelhorCaminho(Mapa_Grid mapa) {
        this.mapa = mapa;
    }

    public int[] getCaminho() {
        return caminho;
    }

    public Set<Integer> getNodosPercorridos() {
        return nodosPercorridos;
    }
    
    protected void adicionarNodoPercorrido(Nodo nodo) {
        nodosPercorridos.add(nodo.x + nodo.y * 1000);
    }
    
    protected boolean ehNodoValido(Nodo nodo) {
    	return
    			nodo.x >= 0 && nodo.y >= 0 && 
    			nodo.x < 1000 && nodo.y < 1000 &&
				mapa.mapa[nodo.y][nodo.x] == 0 && 
				!ehNodoPercorrido(nodo);
    }

    private boolean ehNodoPercorrido(Nodo nodo) {
        return nodosPercorridos.contains(nodo.x + nodo.y * 1000);
    }

    public abstract void buscar(int iniX, int iniY, int objX, int objY);
}
