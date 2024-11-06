import sys
import random
import os 
import time
import argparse
from pymemcache.client.base import Client



place_holder = "placeholder"
word_list_file = "unique_words.txt"

def debug_print(*args):
    if verbose:
        print(*args)

def select_random_words(file_path, num_words):
    with open(file_path, 'r') as file:
        words = file.readlines()
    words = [word.strip() for word in words]
    random_words = random.sample(words, num_words)
    return random_words

if __name__ == "__main__":

    # Create an ArgumentParser object
    parser = argparse.ArgumentParser(description="Process some integers.")

    # Add arguments
    parser.add_argument('--preload_num', type=int, default=3000, help='Number of preload words(default: 3000)')
    parser.add_argument('--test_num', type=int, default=250, help='Number of test words (default: 250)')
    parser.add_argument('--req_num', type=int, default=1000, help='Number of requests (default: 250)')
    parser.add_argument("--get_set_ratio", type=float, default=0.5, help="Ratio of get to set operations (default: 0.5)")
    parser.add_argument("--server_ip", type=str, default="localhost", help="IP of the memcached server (default: localhost)")
    parser.add_argument("--server_port", type=int, default=11211, help="Port of the memcached server (default: 11211)")
    parser.add_argument("--verbose", action="store_true", help="Print debug messages")
    parser.add_argument("--seed", type=int, default=1, help="Seed of random sample")
    parser.add_argument("--repeat", type=int, default=1, help="Request Repeat")
    args = parser.parse_args()

    server_ip = args.server_ip
    server_port = args.server_port

    preload_num = args.preload_num
    test_num = args.test_num
    req_num = args.req_num
    get_set_ratio = args.get_set_ratio
    verbose = args.verbose

    random.seed(args.seed)

    words=[]
    preload_corpus = []
    test_corpus = []

    with open(word_list_file, 'r') as file:
        words = file.readlines()
    words = [word.strip() for word in words]

    preload_corpus = random.sample(words, preload_num)

    test_corpus = [ preload_corpus[random.randrange(test_num)] for i in range(req_num)]
    req_get_num = int(req_num*get_set_ratio)
    req_set_num = req_num - req_get_num
    set_test_corpus = [ preload_corpus[random.randrange(test_num)] for i in range(req_set_num) ]
    get_test_corpus = [ preload_corpus[random.randrange(test_num)] for i in range(req_get_num) ]
    req_list = random.sample(['get','set'], counts=[req_get_num, req_set_num], k=req_num)


    # Preload
    client = Client((server_ip, server_port), no_delay=True)
    client.get('dummy') # wait
    st = time.time()
    for word in preload_corpus:
        client.set(word, "dummy_value",noreply=True)
    client.get('dummy') # wait
    et = time.time()
    client.close()

    client = Client((server_ip, server_port), no_delay=True)
    st = time.time()
    for word in preload_corpus:
        client.set(word, "dummy_value",noreply=False)

    # Async requests
    # for word, req in zip(test_corpus, req_list):
    #     if req == "set":
    #         client.set(word, "dummy_value", noreply=True)
    #     if req == "get":
    #         client.get(word, noreply=True)

    # for word in get_test_corpus:
    #     client.set(word, "dummy_value",noreply=False)

    # client.get('dummy') # wait
    # et = time.time()
    # print(f'{req_num/(et-st):7.f}')

    client.close()

    # SET
    client = Client((server_ip, server_port))
    client.get('dummy') # wait
    st = time.time()
    for word in test_corpus:
        client.set(word, "dummy_value",noreply=True)
    client.get('dummy') # wait
    et = time.time()

    client.close()
    set_rps = len(test_corpus)/(et-st)

    # GET
    client = Client((server_ip, server_port))
    client.get('dummy') # wait
    st = time.time()
    for word in test_corpus:
        client.get(word)
    et = time.time()
    client.close()

    get_rps = len(test_corpus)/(et-st)

    print(f"{preload_num:7d}, {test_num:7d}, {set_rps:10.2f}, {get_rps:10.2f}")

    # print(f'get: {(len(get_test_corpus))/(et-st)} req/sec')

    # for word, req in zip(test_corpus, req_list):
    #     if req == "set":
    #         client.set(word, "dummy_value", no)
    #     if req == "get":
    #         client.get(word)
