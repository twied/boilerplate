/*
 * webcamjava: How to access your web cam with java on linux.
 * Written in 2014 by Tim Wiederhake
 *
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along
 * with this software. If not, see
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;

public class WebCam {
    private static BufferedImage getImage(final InputStream stream) throws IOException {
        final byte[] buffer = new byte[640 * 480 * 3];
        stream.read(buffer);
        return ImageIO.read(new ByteArrayInputStream(buffer));
    }

    public static void main(final String... args) throws IOException {
        final FileInputStream stream = new FileInputStream("/dev/video0");

        final JLabel content = new JLabel(new ImageIcon(getImage(stream)));

        final JFrame frame = new JFrame("java webcam");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setContentPane(content);
        frame.pack();
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);

        while (true) {
            content.setIcon(new ImageIcon(getImage(stream)));
        }
    }
}
