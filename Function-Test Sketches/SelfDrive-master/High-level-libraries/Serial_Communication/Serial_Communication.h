/**
 * This method gets the actual number from the number with
 * the counter and the filler value placed before the number
 *
 * @param number_Received
 */
long receiveData (long number_Received){
    long actual_number = 0;
    
    int counter = number_Received / 10000;
    int valid_data = 4 - counter;
    if (valid_data == 0)
        actual_number = 0;
    else if (valid_data == 1)
        actual_number = number_Received % 10;
    else if(valid_data == 2)
        actual_number = number_Received % 100;
    else if(valid_data ==3)
        actual_number = number_Received % 1000;
    else
        actual_number = number_Received % 10000;
    
    return actual_number;
}

/**
 * This method fills the number sent with the filler
 * value in front of that number
 *
 * @param number sent
 */
long sendData (long number_Sent){
    
    long send_Value = 0;
    if(number_Sent == 0)
        send_Value = 49999;
    else if(number_Sent < 10)
        send_Value = 39990 + number_Sent;
    else if(number_Sent < 100)
        send_Value = 29900 + number_Sent;
    else if(number_Sent < 1000)
        send_Value = 19000 + number_Sent;
    else {
        send_Value = 90000 + number_Sent;
    }
    
    return send_Value;
}

// Scaling down of the turning angle
int scaleDownAngle (long angle){
    if(angle >= -25 && angle <= 25)
        return 4;
    
    else if(angle > 25 && angle <= 40)
        return 5;
    
    else if (angle > 40 && angle <= 55)
        return 6;
    
    else if (angle > 55 && angle <= 70)
        return 7;
    
    else if (angle >70)
        return 8;
    
    else if(angle < -25 && angle >= -40)
        return 3;
    
    else if(angle < -40 && angle >= -55)
        return 2;
    
    else if(angle < -55 && angle >= -70)
        return 1;
    
    else
        return 0;
    
}


