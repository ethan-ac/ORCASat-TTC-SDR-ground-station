#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: gndstation_hiers
# Author: ethan
# GNU Radio version: v3.8.2.0-79-gd56bdd16

from distutils.version import StrictVersion

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print("Warning: failed to XInitThreads()")

import os
import sys
sys.path.append(os.environ.get('GRC_HIER_PATH', os.path.expanduser('~/.grc_gnuradio')))

from PyQt5 import Qt
from gnuradio import qtgui
from gnuradio.filter import firdes
import sip
from decoder_hier import decoder_hier  # grc-generated hier_block
from demodulator_hier import demodulator_hier  # grc-generated hier_block
from gnuradio import blocks
from gnuradio import gr
import signal
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
from gnuradio import uhd
import time
from gnuradio.qtgui import Range, RangeWidget
from modulator_hier import modulator_hier  # grc-generated hier_block
from openlst_encode_hier import openlst_encode_hier  # grc-generated hier_block
from ptt_hier import ptt_hier  # grc-generated hier_block
from zmq_encode_hier import zmq_encode_hier  # grc-generated hier_block
import satellites
import zpdu

from gnuradio import qtgui

class gndstation_hiers(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "gndstation_hiers")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("gndstation_hiers")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "gndstation_hiers")

        try:
            if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
                self.restoreGeometry(self.settings.value("geometry").toByteArray())
            else:
                self.restoreGeometry(self.settings.value("geometry"))
        except:
            pass

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 48000
        self.toggle = toggle = False
        self.tgain = tgain = 50
        self.taps = taps = firdes.low_pass(5.0, samp_rate, 20000,2000, firdes.WIN_HAMMING, 6.76)
        self.sync_word = sync_word = "11010011100100011101001110010001"
        self.samp_rate_usrp = samp_rate_usrp = 2000000
        self.samp_per_sym = samp_per_sym = 5
        self.rgain = rgain = 60
        self.preamble_bytes = preamble_bytes = 2
        self.pre_tx = pre_tx = 50
        self.post_tx = post_tx = 50
        self.gain_mu = gain_mu = 0.175*3
        self.freq = freq = 437002500
        self.cable_file = cable_file = "/dev/ptt"
        self.bit_per_sym = bit_per_sym = 1

        ##################################################
        # Blocks
        ##################################################
        _toggle_check_box = Qt.QCheckBox('Mute')
        self._toggle_choices = {True: True, False: False}
        self._toggle_choices_inv = dict((v,k) for k,v in self._toggle_choices.items())
        self._toggle_callback = lambda i: Qt.QMetaObject.invokeMethod(_toggle_check_box, "setChecked", Qt.Q_ARG("bool", self._toggle_choices_inv[i]))
        self._toggle_callback(self.toggle)
        _toggle_check_box.stateChanged.connect(lambda i: self.set_toggle(self._toggle_choices[bool(i)]))
        self.top_grid_layout.addWidget(_toggle_check_box)
        self._tgain_range = Range(0, 100, 1, 50, 200)
        self._tgain_win = RangeWidget(self._tgain_range, self.set_tgain, 'tgain', "counter_slider", float)
        self.top_grid_layout.addWidget(self._tgain_win)
        self._rgain_range = Range(0, 100, 1, 60, 200)
        self._rgain_win = RangeWidget(self._rgain_range, self.set_rgain, 'rgain', "counter_slider", float)
        self.top_grid_layout.addWidget(self._rgain_win)
        self.zpdu_sub_pull_socket_0_0 = zpdu.sub_pull_socket('tcp://127.0.0.1:44444', 7, True, 100)
        self.zpdu_sub_pull_socket_0 = zpdu.sub_pull_socket('tcp://127.0.0.1:66666', 2, False, 100)
        self.zpdu_pub_push_socket_0 = zpdu.pub_push_socket('tcp://127.0.0.1:55555', 1, False, 100)
        self.zmq_encode_hier_0 = zmq_encode_hier(
            bit_per_sym=bit_per_sym,
            crc16_in_len=False,
            preamble_bytes=preamble_bytes,
            samp_per_sym=samp_per_sym,
            sync_word=sync_word,
        )
        self.uhd_usrp_source_0 = uhd.usrp_source(
            ",".join(("serial=308F983", "")),
            uhd.stream_args(
                cpu_format="fc32",
                args='',
                channels=list(range(0,1)),
            ),
        )
        self.uhd_usrp_source_0.set_center_freq(freq, 0)
        self.uhd_usrp_source_0.set_gain(rgain, 0)
        self.uhd_usrp_source_0.set_antenna('RX2', 0)
        self.uhd_usrp_source_0.set_bandwidth(samp_rate_usrp, 0)
        self.uhd_usrp_source_0.set_samp_rate(samp_rate_usrp)
        # No synchronization enforced.
        self.uhd_usrp_sink_0 = uhd.usrp_sink(
            ",".join(("serial=308F983", "")),
            uhd.stream_args(
                cpu_format="fc32",
                args='',
                channels=list(range(0,1)),
            ),
            '',
        )
        self.uhd_usrp_sink_0.set_center_freq(freq, 0)
        self.uhd_usrp_sink_0.set_gain(tgain, 0)
        self.uhd_usrp_sink_0.set_antenna('TX/RX', 0)
        self.uhd_usrp_sink_0.set_bandwidth(samp_rate_usrp, 0)
        self.uhd_usrp_sink_0.set_samp_rate(samp_rate_usrp)
        # No synchronization enforced.
        self.satellites_print_timestamp_0 = satellites.print_timestamp('%Y-%m-%d %H:%M:%S:%f', True)
        self.qtgui_time_sink_x_2_0 = qtgui.time_sink_c(
            262144, #size
            samp_rate_usrp, #samp_rate
            "receive", #name
            1 #number of inputs
        )
        self.qtgui_time_sink_x_2_0.set_update_time(0.10)
        self.qtgui_time_sink_x_2_0.set_y_axis(-1, 1)

        self.qtgui_time_sink_x_2_0.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_2_0.enable_tags(True)
        self.qtgui_time_sink_x_2_0.set_trigger_mode(qtgui.TRIG_MODE_AUTO, qtgui.TRIG_SLOPE_POS, 0.1, 0, 0, "")
        self.qtgui_time_sink_x_2_0.enable_autoscale(False)
        self.qtgui_time_sink_x_2_0.enable_grid(False)
        self.qtgui_time_sink_x_2_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_2_0.enable_control_panel(True)
        self.qtgui_time_sink_x_2_0.enable_stem_plot(False)


        labels = ['Signal 1', 'Signal 2', 'Signal 3', 'Signal 4', 'Signal 5',
            'Signal 6', 'Signal 7', 'Signal 8', 'Signal 9', 'Signal 10']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ['blue', 'red', 'green', 'black', 'cyan',
            'magenta', 'yellow', 'dark red', 'dark green', 'dark blue']
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]
        styles = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1]


        for i in range(2):
            if len(labels[i]) == 0:
                if (i % 2 == 0):
                    self.qtgui_time_sink_x_2_0.set_line_label(i, "Re{{Data {0}}}".format(i/2))
                else:
                    self.qtgui_time_sink_x_2_0.set_line_label(i, "Im{{Data {0}}}".format(i/2))
            else:
                self.qtgui_time_sink_x_2_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_2_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_2_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_2_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_2_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_2_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_2_0_win = sip.wrapinstance(self.qtgui_time_sink_x_2_0.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_time_sink_x_2_0_win)
        self.qtgui_time_sink_x_2 = qtgui.time_sink_c(
            262144, #size
            samp_rate_usrp, #samp_rate
            "send", #name
            1 #number of inputs
        )
        self.qtgui_time_sink_x_2.set_update_time(0.10)
        self.qtgui_time_sink_x_2.set_y_axis(-1, 1)

        self.qtgui_time_sink_x_2.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_2.enable_tags(True)
        self.qtgui_time_sink_x_2.set_trigger_mode(qtgui.TRIG_MODE_AUTO, qtgui.TRIG_SLOPE_POS, 0.1, 0, 0, "")
        self.qtgui_time_sink_x_2.enable_autoscale(False)
        self.qtgui_time_sink_x_2.enable_grid(False)
        self.qtgui_time_sink_x_2.enable_axis_labels(True)
        self.qtgui_time_sink_x_2.enable_control_panel(True)
        self.qtgui_time_sink_x_2.enable_stem_plot(False)


        labels = ['Signal 1', 'Signal 2', 'Signal 3', 'Signal 4', 'Signal 5',
            'Signal 6', 'Signal 7', 'Signal 8', 'Signal 9', 'Signal 10']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ['blue', 'red', 'green', 'black', 'cyan',
            'magenta', 'yellow', 'dark red', 'dark green', 'dark blue']
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]
        styles = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1]


        for i in range(2):
            if len(labels[i]) == 0:
                if (i % 2 == 0):
                    self.qtgui_time_sink_x_2.set_line_label(i, "Re{{Data {0}}}".format(i/2))
                else:
                    self.qtgui_time_sink_x_2.set_line_label(i, "Im{{Data {0}}}".format(i/2))
            else:
                self.qtgui_time_sink_x_2.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_2.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_2.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_2.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_2.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_2.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_2_win = sip.wrapinstance(self.qtgui_time_sink_x_2.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_time_sink_x_2_win)
        self.qtgui_freq_sink_x_1 = qtgui.freq_sink_c(
            1024, #size
            firdes.WIN_BLACKMAN_hARRIS, #wintype
            433000000, #fc
            samp_rate_usrp, #bw
            "send", #name
            1
        )
        self.qtgui_freq_sink_x_1.set_update_time(0.10)
        self.qtgui_freq_sink_x_1.set_y_axis(-140, 10)
        self.qtgui_freq_sink_x_1.set_y_label('Relative Gain', 'dB')
        self.qtgui_freq_sink_x_1.set_trigger_mode(qtgui.TRIG_MODE_FREE, 0.0, 0, "")
        self.qtgui_freq_sink_x_1.enable_autoscale(False)
        self.qtgui_freq_sink_x_1.enable_grid(False)
        self.qtgui_freq_sink_x_1.set_fft_average(1.0)
        self.qtgui_freq_sink_x_1.enable_axis_labels(True)
        self.qtgui_freq_sink_x_1.enable_control_panel(True)



        labels = ['', '', '', '', '',
            '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
            "magenta", "yellow", "dark red", "dark green", "dark blue"]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]

        for i in range(1):
            if len(labels[i]) == 0:
                self.qtgui_freq_sink_x_1.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_freq_sink_x_1.set_line_label(i, labels[i])
            self.qtgui_freq_sink_x_1.set_line_width(i, widths[i])
            self.qtgui_freq_sink_x_1.set_line_color(i, colors[i])
            self.qtgui_freq_sink_x_1.set_line_alpha(i, alphas[i])

        self._qtgui_freq_sink_x_1_win = sip.wrapinstance(self.qtgui_freq_sink_x_1.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_freq_sink_x_1_win)
        self.qtgui_freq_sink_x_0 = qtgui.freq_sink_c(
            1024, #size
            firdes.WIN_BLACKMAN_hARRIS, #wintype
            freq, #fc
            samp_rate_usrp, #bw
            "receive", #name
            1
        )
        self.qtgui_freq_sink_x_0.set_update_time(0.10)
        self.qtgui_freq_sink_x_0.set_y_axis(-140, 10)
        self.qtgui_freq_sink_x_0.set_y_label('Relative Gain', 'dB')
        self.qtgui_freq_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, 0.0, 0, "")
        self.qtgui_freq_sink_x_0.enable_autoscale(False)
        self.qtgui_freq_sink_x_0.enable_grid(False)
        self.qtgui_freq_sink_x_0.set_fft_average(0.2)
        self.qtgui_freq_sink_x_0.enable_axis_labels(True)
        self.qtgui_freq_sink_x_0.enable_control_panel(True)



        labels = ['', '', '', '', '',
            '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
            "magenta", "yellow", "dark red", "dark green", "dark blue"]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]

        for i in range(1):
            if len(labels[i]) == 0:
                self.qtgui_freq_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_freq_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_freq_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_freq_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_freq_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_freq_sink_x_0_win = sip.wrapinstance(self.qtgui_freq_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_freq_sink_x_0_win)
        self.ptt_hier_0 = ptt_hier(
            cable_file=cable_file,
            post_tx=post_tx,
            pre_tx=pre_tx,
            samp_rate_usrp=samp_rate_usrp,
        )
        self.openlst_encode_hier_0 = openlst_encode_hier(
            bit_per_sym=bit_per_sym,
            preamble_bytes=preamble_bytes,
            samp_per_sym=samp_per_sym,
            sync_word=sync_word,
            system='40',
        )
        self.modulator_hier_0 = modulator_hier(
            samp_per_sym=samp_per_sym,
            samp_rate=samp_rate,
            samp_rate_usrp=samp_rate_usrp,
        )
        self.demodulator_hier_1 = demodulator_hier(
            gain_mu=0.175*3,
            samp_rate=48000,
            samp_rate_usrp=2000000,
        )
        self.decoder_hier_0 = decoder_hier(
            sync_word=sync_word,
        )
        self.blocks_mute_xx_0 = blocks.mute_cc(bool(toggle))
        self.blocks_message_debug_0_1 = blocks.message_debug()
        self.blocks_message_debug_0 = blocks.message_debug()


        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.decoder_hier_0, 'fail'), (self.satellites_print_timestamp_0, 'in'))
        self.msg_connect((self.decoder_hier_0, 'ok'), (self.satellites_print_timestamp_0, 'in'))
        self.msg_connect((self.decoder_hier_0, 'fail'), (self.zpdu_pub_push_socket_0, 'in'))
        self.msg_connect((self.decoder_hier_0, 'ok'), (self.zpdu_pub_push_socket_0, 'in'))
        self.msg_connect((self.openlst_encode_hier_0, 'out'), (self.blocks_message_debug_0, 'print_pdu'))
        self.msg_connect((self.openlst_encode_hier_0, 'out'), (self.modulator_hier_0, 'in'))
        self.msg_connect((self.satellites_print_timestamp_0, 'out'), (self.blocks_message_debug_0_1, 'print_pdu'))
        self.msg_connect((self.zmq_encode_hier_0, 'out'), (self.blocks_message_debug_0, 'print_pdu'))
        self.msg_connect((self.zmq_encode_hier_0, 'out'), (self.modulator_hier_0, 'in'))
        self.msg_connect((self.zpdu_sub_pull_socket_0, 'out'), (self.zmq_encode_hier_0, 'in'))
        self.msg_connect((self.zpdu_sub_pull_socket_0_0, 'out'), (self.openlst_encode_hier_0, 'in'))
        self.connect((self.blocks_mute_xx_0, 0), (self.ptt_hier_0, 0))
        self.connect((self.demodulator_hier_1, 0), (self.decoder_hier_0, 0))
        self.connect((self.modulator_hier_0, 0), (self.blocks_mute_xx_0, 0))
        self.connect((self.ptt_hier_0, 0), (self.qtgui_freq_sink_x_1, 0))
        self.connect((self.ptt_hier_0, 0), (self.qtgui_time_sink_x_2, 0))
        self.connect((self.ptt_hier_0, 0), (self.uhd_usrp_sink_0, 0))
        self.connect((self.uhd_usrp_source_0, 0), (self.demodulator_hier_1, 0))
        self.connect((self.uhd_usrp_source_0, 0), (self.qtgui_freq_sink_x_0, 0))
        self.connect((self.uhd_usrp_source_0, 0), (self.qtgui_time_sink_x_2_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "gndstation_hiers")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.modulator_hier_0.set_samp_rate(self.samp_rate)

    def get_toggle(self):
        return self.toggle

    def set_toggle(self, toggle):
        self.toggle = toggle
        self._toggle_callback(self.toggle)
        self.blocks_mute_xx_0.set_mute(bool(self.toggle))

    def get_tgain(self):
        return self.tgain

    def set_tgain(self, tgain):
        self.tgain = tgain
        self.uhd_usrp_sink_0.set_gain(self.tgain, 0)

    def get_taps(self):
        return self.taps

    def set_taps(self, taps):
        self.taps = taps

    def get_sync_word(self):
        return self.sync_word

    def set_sync_word(self, sync_word):
        self.sync_word = sync_word
        self.decoder_hier_0.set_sync_word(self.sync_word)
        self.openlst_encode_hier_0.set_sync_word(self.sync_word)
        self.zmq_encode_hier_0.set_sync_word(self.sync_word)

    def get_samp_rate_usrp(self):
        return self.samp_rate_usrp

    def set_samp_rate_usrp(self, samp_rate_usrp):
        self.samp_rate_usrp = samp_rate_usrp
        self.modulator_hier_0.set_samp_rate_usrp(self.samp_rate_usrp)
        self.ptt_hier_0.set_samp_rate_usrp(self.samp_rate_usrp)
        self.qtgui_freq_sink_x_0.set_frequency_range(self.freq, self.samp_rate_usrp)
        self.qtgui_freq_sink_x_1.set_frequency_range(433000000, self.samp_rate_usrp)
        self.qtgui_time_sink_x_2.set_samp_rate(self.samp_rate_usrp)
        self.qtgui_time_sink_x_2_0.set_samp_rate(self.samp_rate_usrp)
        self.uhd_usrp_sink_0.set_samp_rate(self.samp_rate_usrp)
        self.uhd_usrp_sink_0.set_bandwidth(self.samp_rate_usrp, 0)
        self.uhd_usrp_source_0.set_samp_rate(self.samp_rate_usrp)
        self.uhd_usrp_source_0.set_bandwidth(self.samp_rate_usrp, 0)

    def get_samp_per_sym(self):
        return self.samp_per_sym

    def set_samp_per_sym(self, samp_per_sym):
        self.samp_per_sym = samp_per_sym
        self.modulator_hier_0.set_samp_per_sym(self.samp_per_sym)
        self.openlst_encode_hier_0.set_samp_per_sym(self.samp_per_sym)
        self.zmq_encode_hier_0.set_samp_per_sym(self.samp_per_sym)

    def get_rgain(self):
        return self.rgain

    def set_rgain(self, rgain):
        self.rgain = rgain
        self.uhd_usrp_source_0.set_gain(self.rgain, 0)

    def get_preamble_bytes(self):
        return self.preamble_bytes

    def set_preamble_bytes(self, preamble_bytes):
        self.preamble_bytes = preamble_bytes
        self.openlst_encode_hier_0.set_preamble_bytes(self.preamble_bytes)
        self.zmq_encode_hier_0.set_preamble_bytes(self.preamble_bytes)

    def get_pre_tx(self):
        return self.pre_tx

    def set_pre_tx(self, pre_tx):
        self.pre_tx = pre_tx
        self.ptt_hier_0.set_pre_tx(self.pre_tx)

    def get_post_tx(self):
        return self.post_tx

    def set_post_tx(self, post_tx):
        self.post_tx = post_tx
        self.ptt_hier_0.set_post_tx(self.post_tx)

    def get_gain_mu(self):
        return self.gain_mu

    def set_gain_mu(self, gain_mu):
        self.gain_mu = gain_mu

    def get_freq(self):
        return self.freq

    def set_freq(self, freq):
        self.freq = freq
        self.qtgui_freq_sink_x_0.set_frequency_range(self.freq, self.samp_rate_usrp)
        self.uhd_usrp_sink_0.set_center_freq(self.freq, 0)
        self.uhd_usrp_source_0.set_center_freq(self.freq, 0)

    def get_cable_file(self):
        return self.cable_file

    def set_cable_file(self, cable_file):
        self.cable_file = cable_file
        self.ptt_hier_0.set_cable_file(self.cable_file)

    def get_bit_per_sym(self):
        return self.bit_per_sym

    def set_bit_per_sym(self, bit_per_sym):
        self.bit_per_sym = bit_per_sym
        self.openlst_encode_hier_0.set_bit_per_sym(self.bit_per_sym)
        self.zmq_encode_hier_0.set_bit_per_sym(self.bit_per_sym)





def main(top_block_cls=gndstation_hiers, options=None):

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()

    tb.start()

    tb.show()

    def sig_handler(sig=None, frame=None):
        Qt.QApplication.quit()

    signal.signal(signal.SIGINT, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)

    timer = Qt.QTimer()
    timer.start(500)
    timer.timeout.connect(lambda: None)

    def quitting():
        tb.stop()
        tb.wait()

    qapp.aboutToQuit.connect(quitting)
    qapp.exec_()

if __name__ == '__main__':
    main()
