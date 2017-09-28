package com.myhand.shtcdatafile;

import android.util.Log;

import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Created by wenha_000 on 2017-09-28.
 */
public class DataBlockTest {
    private static final String tag=DataBlockTest.class.getSimpleName();
    private DataBlock dataBlock;

    @Before
    public void setUp() throws Exception {
        dataBlock = new DataBlock();
        dataBlock.setFieldsLength(new byte[]{2,2,2});
    }

    @Test
    public void getData() throws Exception {
        //System.out.println(String.format("Data:%s",dataBlock.getData()));
        assertEquals("000000",dataBlock.getData(),0);
    }

}