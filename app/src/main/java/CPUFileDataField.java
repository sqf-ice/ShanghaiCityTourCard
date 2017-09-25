/**
 * Created by wenha_000 on 2017-09-06.
 */

public class CPUFileDataField {
    private byte pos;
    private byte length;

    public CPUFileDataField() {
    }

    public CPUFileDataField(byte pos, byte length) {
        this.pos = pos;
        this.length = length;
    }

    public byte getPos() {
        return pos;
    }

    public void setPos(byte pos) {
        this.pos = pos;
    }

    public byte getLength() {
        return length;
    }

    public void setLength(byte length) {
        this.length = length;
    }
    //just a test with git
}
