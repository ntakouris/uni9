import os
import string
from collections import Counter
import math

K = 3 # user input

def binom(x, y):
    if y == x:
        return 1
    elif y == 1:
        return x
    elif y > x:  
        return 0
    else: 
        a = math.factorial(x)
        b = math.factorial(y)
        c = math.factorial(x-y)  
        div = a // (b * c)
        return div


pathjoin = os.path.join

docs = pathjoin(os.path.dirname(__file__), './documents')

doc_names = []
doc_counts = []
doc_similarity = {} # key is simkey() and value is cosine score

file_num = 0
for doc in os.listdir(docs):
    if not doc.lower().endswith('.txt'):
        continue
    
    print(f'\nReading {doc} \n')
    words = open(pathjoin(docs, doc)).read().lower().split()
    # strip punctuation ['(' , ')' , ',' , ...] (optional) and create word frequency map
    count = Counter(map(lambda w: w.translate(str.maketrans('', '', string.punctuation)), words))
    print(count)

    doc_names.append(doc) # store for use later
    doc_counts.append(count)
    file_num = file_num + 1

def simkey(i, j): # dict key format for hashing + readability
    return f'{i} - {j}'

def veclen(v): # accumulates the sum of each element squared of given enumerable/iterable v
    return sum(map(lambda x:x*x, v))

# calculate all similarities and store at doc_similarity[key], values range [0, 1]
for i in range(0, file_num):
    for j in range(i, file_num):
        if i == j:
            continue
        
        k = simkey(doc_names[i], doc_names[j])
        
        d1_count = doc_counts[i]
        d2_count = doc_counts[j]

        d1_l = veclen(d1_count.values())
        d2_l = veclen(d2_count.values())

        # cosine similarity formula
        inner_product = sum(map(lambda w1: d1_count[w1] * d2_count[w1], d1_count))
        score = inner_product / (d1_l * d2_l)

        doc_similarity[k] = score
        print(f'\nSimilarity for {k}: {score}')


# Top K most similar
k_lim = binom(file_num, 2)

K = k_lim if K > k_lim else K

print(f'\n\n== Top {K} similar documents ==\n\n')

# sort based on value, slice top K to print
print(sorted(doc_similarity.items(), key = lambda kv: kv[1], reverse = True)[0:K])
