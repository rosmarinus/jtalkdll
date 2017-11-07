import jtalk.JTalkJna;
import com.sun.jna.Platform;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.Robot;
import javax.swing.DefaultComboBoxModel;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.LayoutFocusTraversalPolicy;
import javax.swing.SwingUtilities;


public class JtdJnaJavaSwing extends JFrame {

    JComboBox<String> comboBox;
    JTextArea textArea;
    JScrollPane scrollpane;
    JButton buttonSay;
    JButton buttonStop;
    JTalkJna tts;

    class MyFocusPolicy extends LayoutFocusTraversalPolicy {
        @Override
        public Component getComponentAfter(Container container, Component component) {
            if (component == comboBox) {
                return textArea;
            } else if (component == textArea) {
                return buttonSay;
            } else if (component == buttonSay) {
                return buttonStop;
            } else if (component == buttonStop) {
                return comboBox;
            } else {
                return super.getComponentAfter(container, component);
            }
        }

        @Override
        public Component getComponentBefore(Container container, Component component) {
            if (component == comboBox) {
                return buttonStop;
            } else if (component == textArea) {
                return comboBox;
            } else if (component == buttonSay) {
                return textArea;
            } else if (component == buttonStop) {
                return buttonSay;
            } else {
                return super.getComponentBefore(container, component);
            }
        }
    }

    public JtdJnaJavaSwing() {
        super("発声テスト");

        try {
            tts = new JTalkJna();
        } catch (Exception e) {
            return;
        }

        GridBagLayout layout = new GridBagLayout();
        getContentPane().setLayout(layout);

        DefaultComboBoxModel<String> model = new DefaultComboBoxModel<String>();
        tts.getVoices().forEach(v -> model.addElement(v.name));

        tts.addVoiceListChangedListener( event -> {
                try {
                    model.removeAllElements();
                    String name = tts.getVoiceName();
                    tts.getVoices().forEach(v -> model.addElement(v.name));
                    comboBox.setSelectedItem(name);
                } catch (Exception e) {
                }
            }
        );

        comboBox = new JComboBox<String>(model);
        comboBox.setPreferredSize(new Dimension(218, 20));
        comboBox.setMaximumRowCount(16);
        try {
            comboBox.setSelectedItem(tts.getVoiceName());
        } catch (Exception e) {}
        comboBox.addActionListener( event -> {
                try {
                    tts.setVoiceName((String) comboBox.getSelectedItem());
                } catch (Exception e) {
                }
            }
        );
        GridBagConstraints gbcComboBox = new GridBagConstraints();
        gbcComboBox.gridy = 0;
        gbcComboBox.gridx = 0;
        gbcComboBox.gridwidth = 2;
        gbcComboBox.gridheight = 1;
        gbcComboBox.anchor = GridBagConstraints.CENTER;
        gbcComboBox.fill = GridBagConstraints.HORIZONTAL;
        gbcComboBox.insets = new Insets(2, 8, 0, 8);
        layout.setConstraints(comboBox, gbcComboBox);
        getContentPane().add(comboBox);

        textArea = new JTextArea("何か入力してください。");
        textArea.selectAll();
        textArea.addKeyListener(
            new KeyListener() {
                public void keyPressed(KeyEvent event) {
                    try {
                        if (event.getKeyCode() == KeyEvent.VK_TAB) {

                            int mod = event.getModifiersEx();
                            if ((event.getModifiersEx() & InputEvent.SHIFT_DOWN_MASK) != 0) {
                                comboBox.requestFocusInWindow();
                            } else {
                                buttonSay.requestFocusInWindow();
                            }
                        }
                    } catch (Exception e) {
                    }
                }

                public void keyTyped(KeyEvent event) {
                }

                public void keyReleased(KeyEvent event) {
                }
            }
        );


        scrollpane = new JScrollPane(textArea);
        scrollpane.setPreferredSize(new Dimension(218, 50));
        GridBagConstraints gbcScrollpane = new GridBagConstraints();
        gbcScrollpane.gridy = 1;
        gbcScrollpane.gridx = 0;
        gbcScrollpane.gridwidth = 2;
        gbcScrollpane.gridheight = 1;
        gbcScrollpane.anchor = GridBagConstraints.CENTER;
        gbcScrollpane.fill = GridBagConstraints.HORIZONTAL;
        gbcScrollpane.insets = new Insets(7, 8, 0, 8);
        layout.setConstraints(scrollpane, gbcScrollpane);
        getContentPane().add(scrollpane);

        buttonSay = new JButton("発声");
        buttonSay.setPreferredSize(new Dimension(98, 25));
        buttonSay.addActionListener( event -> {
                try {
                    String text = textArea.getText();
                    tts.speakAsync(text);
                } catch (Exception e) {
                }
            }
        );

        buttonSay.addKeyListener(
            new KeyListener() {
                public void keyPressed(KeyEvent event) {
                    try {
                        if (event.getKeyCode() == KeyEvent.VK_ENTER) {
                            Robot robot = new Robot();
                            robot.keyPress(KeyEvent.VK_SPACE);
                            event.consume();
                        }
                    } catch (Exception e) {
                    }
                }

                public void keyTyped(KeyEvent event) {}
 
                public void keyReleased(KeyEvent event) {
                    try {
                        if (event.getKeyCode() == KeyEvent.VK_ENTER) {
                            Robot robot = new Robot();
                            robot.keyRelease(KeyEvent.VK_SPACE);
                            event.consume();
                        }
                    } catch (Exception e) {
                    }
                }
            }
        );

        GridBagConstraints gbcButtonSay = new GridBagConstraints();
        gbcButtonSay.gridy = 2;
        gbcButtonSay.gridx = 0;
        gbcButtonSay.anchor = GridBagConstraints.WEST;
        gbcButtonSay.gridwidth = 1;
        gbcButtonSay.gridheight = 1;
        gbcButtonSay.insets = new Insets(8, 8, 5, 0);
        layout.setConstraints(buttonSay, gbcButtonSay);
        getContentPane().add(buttonSay);

        buttonStop = new JButton("停止");
        buttonStop.setPreferredSize(new Dimension(98, 25));
        buttonStop.addActionListener( event -> {
                try {
                    if (tts.isSpeaking()) {
                        tts.stop();
                    }
                } catch (Exception e) {
                }
            }
        );
        buttonStop.addKeyListener(
            new KeyListener() {
                public void keyPressed(KeyEvent event) {
                    try {
                        if (event.getKeyCode() == KeyEvent.VK_ENTER) {
                            Robot robot = new Robot();
                            robot.keyPress(KeyEvent.VK_SPACE);
                        }
                    } catch (Exception e) {
                    }
                }

                public void keyTyped(KeyEvent event) {
                }

                public void keyReleased(KeyEvent event) {
                    try {
                        if (event.getKeyCode() == KeyEvent.VK_ENTER) {
                            Robot robot = new Robot();
                            robot.keyRelease(KeyEvent.VK_SPACE);
                        }
                    } catch (Exception e) {
                    }
                }
            }
        );

        GridBagConstraints gbcButtonStop = new GridBagConstraints();
        gbcButtonStop.gridy = 2;
        gbcButtonStop.gridx = 1;
        gbcButtonStop.anchor = GridBagConstraints.EAST;
        gbcButtonStop.gridwidth = 1;
        gbcButtonStop.gridheight = 1;
        gbcButtonStop.insets = new Insets(8, 0, 5, 8);
        layout.setConstraints(buttonStop, gbcButtonStop);
        getContentPane().add(buttonStop);

        setFocusTraversalPolicy(new MyFocusPolicy());
        textArea.requestFocusInWindow();
    }

    public static void main(String[] args) {
        JFrame frame = new JtdJnaJavaSwing();
        frame.setVisible(true);
        frame.setBounds(200, 200, 240, 150);
        frame.setResizable(true);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }
}
