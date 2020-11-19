import os, sys
from PyQt5 import QtCore, QtWidgets

class MyApp(QtWidgets.QWidget):
    def __init__(self, base_path, batch):
        self._base_path = base_path 
        self._ui_path = os.path.join(self._base_path, 'ui')
        self._func_path = os.path.join(self._base_path, 'func')
        self._macro_path = os.path.join(self._base_path, 'macro')
        self._sample_path = os.path.join(self._base_path, 'samples')
        self._output_path = os.path.join(self._base_path, 'output')
        self._config = []
        self.batch = batch
        self.cfg = ''
        self.cmd = ''
       
        if self.batch:
            self.initUI()
        else:
            super(QtWidgets.QWidget,self).__init__()
            self.initGUI()

    from batch import _select_command
    from batch import _run_command

    from gui import _make_layout
    from gui import _make_qle_with_lbl
    from gui import _make_cfg_btn
    from gui import _make_cmd_btn
    from gui import _make_cfg_combo
    from gui import _make_cmd_combo

    from action import _get_config
    from action import _run_cmd
    
    def initGUI(self):
        self._make_layout()
        self.setWindowTitle('ttbb lep+jet analyzer')
        
        self.resize(1200,900)
        qr = self.frameGeometry()
        cp = QtWidgets.QDesktopWidget().availableGeometry().center()
        qr.moveCenter(cp)
        self.move(qr.topLeft())
        
        self.show()

    def initUI(self):
        self._set_config()
        self.cmd = self._select_command()
        self._run_command(self.cmd)
