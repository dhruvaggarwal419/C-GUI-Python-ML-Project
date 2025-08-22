import sys
import pandas as pd
import os
import io
os.environ["TF_ENABLE_ONEDNN_OPTS"] = "0"
os.environ["TF_CPP_MIN_LOG_LEVEL"] = "3"
import tensorflow as tf
from tensorflow import keras
import pickle
sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8', errors='replace')
sys.stderr = io.TextIOWrapper(sys.stderr.buffer, encoding='utf-8', errors='replace')
script_dir = os.path.dirname(os.path.abspath(__file__))
model_path = os.path.join(script_dir, "expressions.keras")
tokenizer_path = os.path.join(script_dir, "tokenizer.pkl")


model = keras.models.load_model(model_path)
with open(tokenizer_path, "rb") as f:
    tokenizer = pickle.load(f)

if len(sys.argv) < 2:
    print("Please provide CSV file path")
    sys.exit(1)

csv_file = sys.argv[1]
data = pd.read_csv(csv_file, header=None, names=["expression"])
import pickle
from tensorflow.keras.preprocessing.sequence import pad_sequences
max_len = 20  
sequences = tokenizer.texts_to_sequences(data["expression"])
pad_seq = pad_sequences(sequences, maxlen=max_len, padding="post")

predictions = model.predict(pad_seq)

for expr, pred in zip(data["expression"], predictions):
    if pred >= 0.5:
        try:
            result = eval(expr)
            print(f"{expr} -> Valid, Result = {result}")
        except Exception as e:
            print(f"{expr} -> Error while evaluating: {e}")
    else:
        print(f"{expr} -> Invalid")