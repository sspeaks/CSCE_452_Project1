import java.awt.BorderLayout;
import java.awt.Dimension;

import javax.swing.JFrame;
import javax.swing.JPanel;

public class Run {

	public static void main(String[] args) throws InterruptedException {
		JPanel pane;
		SPath maze = new SPath();
		JFrame frame = new JFrame("maze");
		JPanel content = new JPanel(new BorderLayout());
		frame.setContentPane(content);
		pane = maze.pane;
		content.add(pane, BorderLayout.CENTER);

		System.out.println(pane.getBorder().toString());

		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setPreferredSize(new Dimension(507, 540));
		frame.setMaximumSize(new Dimension(507, 540));
		frame.setMinimumSize(new Dimension(500, 500));
		frame.setResizable(false);
		System.out.println(frame.getSize().toString());

		frame.pack();
		frame.setVisible(true);
		/*
		 * Thread genThread = new Thread(maze.gen); genThread.start();
		 */

	}
}
