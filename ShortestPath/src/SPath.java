import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.event.MouseEvent;
import java.lang.reflect.Array;
import java.util.ArrayList;

import javax.swing.BorderFactory;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.event.MouseInputListener;

public class SPath {

	SPathPanel pane;
	ArrayList<Box> list = new ArrayList<Box>();
	int boxNum;
	int initX, initY;
	int initCenterX, initCenterY;
	int[] startcoors = {0,0};
	int[] endcoors = {0,0};
	boolean startDraw = false, endDraw = false;
	public SPath()
			throws InterruptedException {
		Box boxOne = new Box(200,200, 110,150);
		Box boxTwo = new Box(150,150, 85, 350);
		Box boxThree = new Box(100,100, 270, 75);
		list.add(boxOne);
		list.add(boxTwo);
		list.add(boxThree);
		pane = new SPathPanel();
		pane.setBorder(BorderFactory.createLineBorder(Color.black));
		pane.setLayout(new BorderLayout());		
	}
	

	public boolean solve() throws InterruptedException {
		return false;

	}

	class Box 
	{
		private int height;
		private int width;
		private int centerX;
		private int centerY;
		public Box(int width, int height, int centerX, int centerY)
		{
			this.width = width;
			this.height = height;
			this.centerX = centerX;
			this.centerY = centerY;
		}
		public int getLeftSide()
		{
			return centerX - width/2;
		}
		public int getRightSide()
		{
			return centerX + width/2;
		}
		public int getTopSide()
		{
			return centerY - height/2;
		}
		public int getBottomSide()
		{
			return centerY + height/2;
		}
		public boolean checkCollision(int x, int y)
		{
			if(x <= getRightSide() && x >= getLeftSide() && y >= getTopSide() && y <= getBottomSide())
			{
				return true;
			}
			else
				return false;

		}
		public boolean checkBorderCollision(int centerX, int centerY)
		{
			int curCentX = this.centerX;
			this.centerX = centerX;
			int curCentY = this.centerY;
			this.centerY = centerY;
			if(getLeftSide() < 0)
			{this.centerX = curCentX;
			this.centerY = curCentY;
				return true;
			}
			if(getRightSide() > 500)
			{this.centerX = curCentX;
			this.centerY = curCentY;
				return true;
			}
			if(getTopSide() < 0)
			{this.centerX = curCentX;
			this.centerY = curCentY;
				return true;
			}
			if(getBottomSide() > 500)
			{this.centerX = curCentX;
			this.centerY = curCentY;
				return true;
			}
			
			this.centerX = curCentX;
			this.centerY = curCentY;
			
			return false;
		}

		

	}
	
	

	class SPathPanel extends JPanel {
		int height = 500;
		int width = 500;

		public SPathPanel() {
			super(new BorderLayout());
			this.setBackground(Color.WHITE);
			this.setSize(width,height);
			this.setPreferredSize(new Dimension(500,500));
			this.setMaximumSize(new Dimension(500,500));
			this.setMinimumSize(new Dimension(500,500));
			this.addMouseMotionListener(new BoxMove());
			this.addMouseListener(new BoxMove());
			

		}

		public void stackOverflow() {

			JOptionPane
					.showMessageDialog(SPathPanel.this,
							"Stack Overflow\nRecursion is too deep\nReduce rows/columns");
		}

		@Override
		public void paintComponent(Graphics g) {
			super.paintComponent(g);
			
			g.setColor(Color.BLACK);
			for(Box b : list)
			{
				g.drawRect(b.getLeftSide(), b.getTopSide(), b.width, b.height);
			}
			g.setColor(Color.BLUE);
			
			if(startDraw)
			{
				g.fillOval(startcoors[0], startcoors[1], 5, 5);
			}
			g.setColor(Color.RED);
			if(endDraw)
			{
				g.fillOval(endcoors[0], endcoors[1], 5, 5);
			}
			

		}
		public class BoxMove implements MouseInputListener
		{
			
			
			public BoxMove()
			{
				super();
			}
			@Override
			public void mouseClicked(MouseEvent e) {
				
				
			}

			@Override
			public void mousePressed(MouseEvent e) {
				boolean collided = false;
				for(int i = 0; i < list.size(); i++)
				{
					
					System.out.print(e.getX());
					System.out.println(" " + e.getY());
				//	System.out.println("box " + i);
					//System.out.println("boxLeft " + list.get(i).getLeftSide() + " boxright " + list.get(i).getRightSide());
					//System.out.println("boxTop " + list.get(i).getTopSide() + " boxbottom " + list.get(i).getBottomSide());
					if(list.get(i).checkCollision(e.getX(), e.getY()))
					{
						
						collided = true;
						boxNum = i; 
						//System.out.println("Collision Detection at " + boxNum);
						initX = e.getX();
						initY = e.getY();
						initCenterX = list.get(i).centerX;
						initCenterY = list.get(i).centerY;
					}
				}
				if(!collided)
				{
					
						if(!startDraw)
						{
							startcoors[0] = e.getX();
							startcoors[1] = e.getY();
							pane.repaint();
							startDraw = true;
						}
						else if(!endDraw)
						{
							endcoors[0] = e.getX();
							endcoors[1] = e.getY();
							endDraw = true;
							pane.repaint();
						}
						else
						{
							startDraw = false;
							endDraw = false;
							startcoors[0] = e.getX();
							startcoors[1] = e.getY();
							startDraw = true;
							pane.repaint();
						}
									
				}
				
			}
			@Override
			public void mouseDragged(MouseEvent e) {
				//System.out.println("Mouse dragged");
				if(boxNum != -1)
				{
					System.out.println(boxNum);
					if(!list.get(boxNum).checkBorderCollision(initCenterX + (e.getX() - initX), initCenterY + (e.getY() - initY)))
					{
					list.get(boxNum).centerX = initCenterX + (e.getX() - initX);
					list.get(boxNum).centerY = initCenterY + (e.getY() - initY);
					pane.repaint();
					}
				}
			   }

			@Override
			public void mouseReleased(MouseEvent e) {
				System.out.println("Mouse released");
				boxNum = -1;
				initX = -1;
				initY = -1;
				initCenterX = -1;
				initCenterY = -1;
			}

			@Override
			public void mouseEntered(MouseEvent e) {
				
			}

			@Override
			public void mouseExited(MouseEvent e) {
				
			}
			@Override
			public void mouseMoved(MouseEvent e) {
				//System.out.println("Mouse moved");
				/*if(boxNum != -1)
				{
					System.out.println(list.get(boxNum).centerX = initCenterX + (e.getX() - initX));
					System.out.println(list.get(boxNum).centerY = initCenterY + (e.getX() - initY));
				}*/
			}
			
		}
	}
}
