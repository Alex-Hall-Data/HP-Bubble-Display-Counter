# -*- coding: utf-8 -*-
"""
Created on Sun Jan 24 20:29:07 2021

@author: Alex
"""


import yfinance as yf
import time
import serial

ticker = 'GME'


ser = serial.Serial('COM3',9600)

def printShare(stock):
    stock_str = str(stock)
    ser.write(stock_str.encode())
    print(stock)
    time.sleep(.1)
    
    
def mainProgram():
    
    while(1):
        stock = yf.Ticker(ticker)
        data = stock.history()
        last_quote = (data.tail(1)['Close'].iloc[0])
        printShare(int( round(last_quote)))
        time.sleep(9.5)

    
mainProgram()


