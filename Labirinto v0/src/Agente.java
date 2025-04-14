import java.awt.Graphics2D;


public abstract class Agente {
	float X,Y;
	
	public abstract void SimulaSe(int DiffTime);
	public abstract void DesenhaSe(Graphics2D dbg,int XMundo,int YMundo);
	
}
