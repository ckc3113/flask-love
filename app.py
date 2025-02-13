from flask import Flask, send_file

app = Flask(__name__)

@app.route("/")
def home():
    return '''
        <h1 style="text-align: center;">💖 这是给你的爱心 💖</h1>
        <img src="/heart" style="display: block; margin: auto;">
    '''

@app.route("/heart")
def get_heart():
    return send_file("heart.png", mimetype='image/png')

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=10000)
