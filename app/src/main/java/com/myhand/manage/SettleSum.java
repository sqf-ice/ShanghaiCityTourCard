package com.myhand.manage;

/**
 * Created by vincent on 2017/11/12.
 */

public class SettleSum {
    private int count;
    private int sum;

    public SettleSum() {
    }

    public SettleSum(int count, int sum) {
        this.count = count;
        this.sum = sum;
    }

    public int getCount() {
        return count;
    }

    public void setCount(int count) {
        this.count = count;
    }

    public int getSum() {
        return sum;
    }

    public void setSum(int sum) {
        this.sum = sum;
    }
}
