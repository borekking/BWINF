package de.flo.bwinf.schiebeParkplatz;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

public class Writer {

    /*
     * Simple Writer (to write into textfiles) with help of FileWriter
     *
     */

    private FileWriter fileWriter;

    public Writer(String fileName) {
        if (fileName == null) return;

        this.create(fileName);
    }

    public void close() {
        try {
            this.fileWriter.close();
        } catch (IOException ignored) {
        }
    }

    public void write(String str) {
        try {
            this.fileWriter.write(str);
        } catch (IOException ignored) {
        }
    }

    // Gets file and init fileWriter
    private void create(String filename) {
        File file = new File(filename);

        try {
            this.fileWriter = new FileWriter(file);
        } catch (IOException ignored) {
        }
    }
}
