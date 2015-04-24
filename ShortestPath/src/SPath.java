import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.PriorityQueue;

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
	int[] startcoors = { 0, 0 };
	int[] endcoors = { 0, 0 };
	NodeComparator comp = new NodeComparator();
	PriorityQueue<Node> queue = new PriorityQueue<Node>(comp);
	boolean startDraw = false, endDraw = false;

	public SPath() throws InterruptedException {
		Box boxOne = new Box(200, 200, 110, 150);
		Box boxTwo = new Box(150, 150, 85, 350);
		Box boxThree = new Box(100, 100, 270, 75);
		list.add(boxOne);
		list.add(boxTwo);
		list.add(boxThree);
		pane = new SPathPanel();
		pane.setBorder(BorderFactory.createLineBorder(Color.black));
		pane.setLayout(new BorderLayout());
	}

	public ArrayList<Node> solve() {

		ArrayList<Node> visited = new ArrayList<Node>();

		HashMap<Node, Node> cameFrom = new HashMap<Node, Node>();
		Node curNode = new Node(startcoors[0], startcoors[1]);
		curNode.g_x = 0;
		queue.add(curNode);

		double t_g;

		while (!queue.isEmpty() && queue.size() < 1000) {

			// System.out.println("queue size is " + queue.size());
			double min = 500;
			/*
			 * for(Node n : queue) { if( min > n.f_x()) { min = n.f_x(); } }
			 */

			curNode = queue.poll();
			// System.out.println("Min = " + min + " and poll = " +
			// curNode.f_x());
			if (curNode.x == endcoors[0] && curNode.y == endcoors[1]) {
				return getPath(cameFrom, curNode);
			}

			visited.add(curNode);
			for (Node n : createNeighbors(curNode)) {
				if (visited.contains(n)) {
					continue;
				} else {
					t_g = curNode.g_x
							+ Math.sqrt(Math.pow(n.x - curNode.x, 2)
									+ Math.pow(n.y - curNode.y, 2));
					if (!queue.contains(n)) {
						cameFrom.put(n, curNode);
						n.g_x = t_g;
						queue.add(n);
						pane.repaint();
					} else {
						for (Node v : queue) {
							if (v.x == n.x && v.y == n.y) {
								if (t_g < n.g_x) {
									cameFrom.put(n, curNode);
									n.g_x = t_g;

									if (!queue.contains(n)) {
										queue.add(n);
										pane.repaint();
									}
								}
							}
						}
					}
				}
			}
		}
		return null;

	}

	public ArrayList<Node> getPath(HashMap<Node, Node> cameFrom, Node n) {
		Node current = n;
		ArrayList<Node> path = new ArrayList<Node>();
		path.add(current);
		while (cameFrom.containsKey(current)
				&& cameFrom.get(current) != current) {
			current = cameFrom.get(current);
			path.add(current);
			// System.out.println("Infinite Loop");
		}
		return path;
	}

	class Box {
		private int height;
		private int width;
		private int centerX;
		private int centerY;

		public Box(int width, int height, int centerX, int centerY) {
			this.width = width;
			this.height = height;
			this.centerX = centerX;
			this.centerY = centerY;
		}

		public int getLeftSide() {
			return centerX - width / 2;
		}

		public int getRightSide() {
			return centerX + width / 2;
		}

		public int getTopSide() {
			return centerY - height / 2;
		}

		public int getBottomSide() {
			return centerY + height / 2;
		}

		public boolean checkCollision(int x, int y) {
			if (x <= getRightSide() && x >= getLeftSide() && y >= getTopSide()
					&& y <= getBottomSide()) {
				return true;
			} else
				return false;

		}

		public boolean checkBorderCollision(int centerX, int centerY) {
			int curCentX = this.centerX;
			this.centerX = centerX;
			int curCentY = this.centerY;
			this.centerY = centerY;
			if (getLeftSide() < 0) {
				this.centerX = curCentX;
				this.centerY = curCentY;
				return true;
			}
			if (getRightSide() > 500) {
				this.centerX = curCentX;
				this.centerY = curCentY;
				return true;
			}
			if (getTopSide() < 0) {
				this.centerX = curCentX;
				this.centerY = curCentY;
				return true;
			}
			if (getBottomSide() > 500) {
				this.centerX = curCentX;
				this.centerY = curCentY;
				return true;
			}

			this.centerX = curCentX;
			this.centerY = curCentY;

			return false;
		}

	}

	@SuppressWarnings("serial")
	class SPathPanel extends JPanel {
		int height = 500;
		int width = 500;

		public SPathPanel() {
			super(new BorderLayout());
			this.setBackground(Color.WHITE);
			this.setSize(width, height);
			this.setPreferredSize(new Dimension(500, 500));
			this.setMaximumSize(new Dimension(500, 500));
			this.setMinimumSize(new Dimension(500, 500));
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
			for (Box b : list) {
				g.drawRect(b.getLeftSide(), b.getTopSide(), b.width, b.height);
			}
			g.setColor(Color.BLUE);

			if (startDraw) {
				g.fillOval(startcoors[0], startcoors[1], 5, 5);
			}
			g.setColor(Color.RED);
			if (endDraw) {
				g.fillOval(endcoors[0], endcoors[1], 5, 5);
			}

			if (startDraw && endDraw) {
				ArrayList<Node> points = solve();
				g.setColor(Color.BLUE);
				for (Node n : queue) {
					g.fillOval(n.x, n.y, 5, 5);
				}
				if (points != null) {
					g.setColor(Color.RED);
					for (Node n : points) {
						g.fillOval(n.x, n.y, 5, 5);
					}
					queue.clear();
				}
			}

		}

		public class BoxMove implements MouseInputListener {

			public BoxMove() {
				super();
			}

			@Override
			public void mouseClicked(MouseEvent e) {

			}

			@Override
			public void mousePressed(MouseEvent e) {
				boolean collided = false;
				for (int i = 0; i < list.size(); i++) {

					// System.out.print(e.getX());
					// System.out.println(" " + e.getY());
					// System.out.println("box " + i);
					// System.out.println("boxLeft " + list.get(i).getLeftSide()
					// + " boxright " + list.get(i).getRightSide());
					// System.out.println("boxTop " + list.get(i).getTopSide() +
					// " boxbottom " + list.get(i).getBottomSide());
					if (list.get(i).checkCollision(e.getX(), e.getY())) {

						collided = true;
						startDraw = false;
						endDraw = false;
						boxNum = i;
						// System.out.println("Collision Detection at " +
						// boxNum);
						initX = e.getX();
						initY = e.getY();
						initCenterX = list.get(i).centerX;
						initCenterY = list.get(i).centerY;
					}
				}
				if (!collided) {

					if (!startDraw) {
						startcoors[0] = e.getX();
						startcoors[1] = e.getY();
						pane.repaint();
						startDraw = true;
					} else if (!endDraw) {
						endcoors[0] = e.getX();
						endcoors[1] = e.getY();
						endDraw = true;
						pane.repaint();
					} else {
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
				// System.out.println("Mouse dragged");
				if (boxNum != -1) {
					// System.out.println(boxNum);
					if (!list.get(boxNum).checkBorderCollision(
							initCenterX + (e.getX() - initX),
							initCenterY + (e.getY() - initY))) {
						list.get(boxNum).centerX = initCenterX
								+ (e.getX() - initX);
						list.get(boxNum).centerY = initCenterY
								+ (e.getY() - initY);
						pane.repaint();
					}
				}
			}

			@Override
			public void mouseReleased(MouseEvent e) {
				// System.out.println("Mouse released");
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
				// System.out.println("Mouse moved");
				/*
				 * if(boxNum != -1) {
				 * System.out.println(list.get(boxNum).centerX = initCenterX +
				 * (e.getX() - initX));
				 * System.out.println(list.get(boxNum).centerY = initCenterY +
				 * (e.getX() - initY)); }
				 */
			}

		}
	}

	public ArrayList<Node> createNeighbors(Node n) {
		int x = n.x;
		int y = n.y;
		ArrayList<Node> neigh = new ArrayList<Node>();
		int i = 0;
		Node temp;
		boolean aff = true;/*
							 * for(i = 0; i < list.size(); i++) {
							 * if(list.get(i).checkCollision(x+1, y+1)) aff =
							 * false; } if(aff) { temp = new Node(x+1, y+1);
							 * temp.g_x = Math.sqrt(2) + n.g_x; neigh.add(temp);
							 * } aff = true; for(i = 0; i < list.size(); i++) {
							 * if(list.get(i).checkCollision(x-1, y+1)) aff =
							 * false; } if(aff) { temp = new Node(x-1, y+1);
							 * temp.g_x = Math.sqrt(2) + n.g_x; neigh.add(temp);
							 * } aff = true; for(i = 0; i < list.size(); i++) {
							 * if(list.get(i).checkCollision(x+1, y-1)) aff =
							 * false; } if(aff) { temp = new Node(x+1, y-1);
							 * temp.g_x = Math.sqrt(2) + n.g_x; neigh.add(temp);
							 * } aff = true; for(i = 0; i < list.size(); i++) {
							 * if(list.get(i).checkCollision(x-1, y-1)) aff =
							 * false; } if(aff) { temp = new Node(x-1, y-1);
							 * temp.g_x = Math.sqrt(2) + n.g_x; neigh.add(temp);
							 * } aff = true;
							 */
		for (i = 0; i < list.size(); i++) {
			if (list.get(i).checkCollision(x, y + 1))
				aff = false;
		}
		if (aff) {
			temp = new Node(x, y + 1);
			temp.g_x = 1 + n.g_x;
			neigh.add(temp);
		}
		aff = true;
		for (i = 0; i < list.size(); i++) {
			if (list.get(i).checkCollision(x, y - 1))
				aff = false;
		}
		if (aff) {
			temp = new Node(x, y - 1);
			temp.g_x = 1 + n.g_x;
			neigh.add(temp);
		}
		aff = true;
		for (i = 0; i < list.size(); i++) {
			if (list.get(i).checkCollision(x + 1, y))
				aff = false;
		}
		if (aff) {
			temp = new Node(x + 1, y);
			temp.g_x = 1 + n.g_x;
			neigh.add(temp);
		}
		aff = true;
		for (i = 0; i < list.size(); i++) {
			if (list.get(i).checkCollision(x - 1, y))
				aff = false;
		}
		if (aff) {
			temp = new Node(x - 1, y);
			temp.g_x = 1 + n.g_x;
			neigh.add(temp);
		}
		aff = true;

		return neigh;

	}

	public double dist(int x, int y) {
		return Math.sqrt(Math.pow(endcoors[0] - x, 2)
				+ Math.pow(endcoors[1] - y, 2));
	}

	public class Node implements Comparable {
		double h_x; // Distance from goal
		double g_x; // Distance Traveled
		int x, y;

		public Node(int x, int y) {
			this.x = x;
			this.y = y;
			h_x = dist(x, y);
			g_x = -1;
		}

		public double f_x() {
			return h_x + g_x;
		}

		@Override
		public boolean equals(Object obj) {
			if (!(obj instanceof Node))
				return false;
			if (((Node) obj).x == this.x && ((Node) obj).y == this.y)
				return true;
			else
				return false;
		}

		@Override
		public int compareTo(Object arg0) {
			if (((Node) arg0).f_x() > this.f_x())
				return 1;
			if (((Node) arg0).f_x() < this.f_x())
				return 1;
			return 0;
		}
	}

	public class NodeComparator implements Comparator<Node> {

		@Override
		public int compare(Node arg0, Node arg1) {
			if (arg0.f_x() > arg1.f_x())
				return 1;
			if (arg0.f_x() < arg1.f_x())
				return -1;
			return 0;
		}

	}
}
