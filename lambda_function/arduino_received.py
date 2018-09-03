import json
import boto3
import logging
import re
import datetime

#set up database to output the values to
dynamodb = boto3.resource('dynamodb')
Table = dynamodb.Table() #insert name of table here

#set up logging to check functionality
logger = logging.getLogger()
logger.setLevel(logging.INFO)


def read_incoming(info):
    for i in range(0,len(info)):
        stuff = info[i].split(" ")
        yield stuff[0][:-1],stuff[1]


def lambda_handler(event, context):
    
    data = event['data']
    topical = event['topic'].split("/")[1]
    topic = "sensors/" +topical
    returnable = {'topic':topic,'datetime':str(datetime.datetime.now())[:19]}
    
    try:
        info = re.findall("\w+:\s+\d{1,4}.\d{1,4}",data)
        if len(info) != 5:
            raise RuntimeError("Not found appropriate data")
    
        for a,b in read_incoming(info):
            returnable[a] = b
        
        Table.put_item(
        Item =returnable
        )
    except RuntimeError:
        logger.info("Data not correct format, exiting")
    
    