#!/usr/bin/env python
# -*- encoding: utf-8 -*-

import sys
import getpass
import argparse
import sqlite3
import subprocess

parser = argparse.ArgumentParser(description='パスワード管理マネージャー')
parser.add_argument('-t', '--type', required=True, nargs=1, help='1: 登録, 2: 取得')
parser.add_argument('-k', '--key', nargs=1, help='typeで操作したいキー名')

args = parser.parse_args()


class Record:
    def __init__(self, row):
        self.key = row[0]
        self.password = row[1]
        self.usage = row[2]


def input_line():
    return sys.stdin.readline().strip()


def check_table(conn):
    cursor = conn.execute("SELECT * FROM sqlite_master WHERE type='table' and name='%s'" % "pass_manage")
    if not cursor.fetchone():
        conn.execute("CREATE TABLE pass_manage (key text primary key, password text, usage text)")


def regist_password(key, conn):
    current_password = get_password(key, conn)
    if current_password != '':
        print('既に登録済みです。更新しますか？[y/N]')
        answer = input_line()
        if answer != 'y':
            return

    cursor = conn.cursor()
    print('パスワードを入力して下さい')
    p = getpass.getpass()
    print('使用用途を入力して下さい')
    usage = input_line()
    if current_password != '':
        cursor.execute("UPDATE pass_manage SET password = ?, usage = ?  WHERE key = ?", (p, usage, key))
    else:
        print("新規登録します。[key=%s]" % key)
        cursor.execute("INSERT INTO pass_manage(key, password, usage) values (?, ?, ?)",
                       (key, p, usage))
    cursor.close()


def get_password(key, conn):
    cursor = conn.cursor()
    cursor.execute("SELECT password FROM pass_manage WHERE key = '%s'" % key)
    data = cursor.fetchone()
    result = ''
    if not data:
        print('キーの登録がありません')
    else:
        result = data[0]
    cursor.close()
    return result


def select_list(conn):
    cursor = conn.cursor()
    cursor.execute("SELECT key, password, usage FROM pass_manage")
    record_list = [Record(r) for r in cursor.fetchall()]
    no = 0
    for record in record_list:
        no += 1
        print("%s: %s [usage: %s]" % (no, record.key, record.usage))
    print('Noを選択して下さい')
    input_no_txt = input_line()
    record_count = len(record_list)
    while not input_no_txt.isdigit() or int(input_no_txt) > record_count:
        print('入力された値が不正です。再度入力して下さい。')
        input_no_txt = input_line()
    record = record_list[int(input_no_txt) - 1]
    return record.password


def set_clipboard(password):
    if password != '':
        p = subprocess.Popen(['pbcopy'], stdin=subprocess.PIPE)
        p.communicate(password.encode('utf-8'))
        if p.wait() == 0:
            print("クリップボードにコピーしました")
        else:
            print("コピーに失敗しました")

if __name__ == '__main__':
    exec_type = int(args.type[0])

    conn = sqlite3.connect('password_manage.db')
    conn.text_factory = str
    check_table(conn)

    if exec_type != 3 and not args.key:
        raise Exception('対話型モード以外は-k/--keyの入力は必須です')

    if exec_type == 1:
        regist_password(args.key[0], conn)
        conn.commit()
    elif exec_type == 2:
        set_clipboard(get_password(args.key[0], conn))
    elif exec_type == 3:
        set_clipboard(select_list(conn))
    else:
        print('実行タイプが不正です。1: 登録/2: 取得/3: 対話型')

    conn.close()
