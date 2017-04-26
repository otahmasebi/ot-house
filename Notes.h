/*
 *  NodeMCU Lolin LED is pin#2
 * 
    // Send the response to the client, because of the length sending multiple chunks is required
    // beware maximum length for println = 2900? on ESP
    for (int chunk=0;chunk <= (String(html_page).length()/2000);chunk++){
      client.print(String(html_page).substring(0+chunk*2000,2000+chunk*2000));
      delay(1);
    }

ORIGINAL code base by Peter (katz) http://www.esp8266.com/viewtopic.php?f=8&t=4307&start=28

Published Hackster.io by M. Ray Burnette 20161025 https://www.hackster.io/rayburne/projects

 */
