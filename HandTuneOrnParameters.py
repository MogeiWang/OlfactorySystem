"""
    To run this script make sure that in simulation_parameters.py:
    n_gor = number_of_lists ( = number of curves)
    rel_orn_mit = 1
    n_or = 32  -- this is the number of concentration steps on the x-axis

"""

import sys
import simulation_parameters
import os
import time
import MergeSpikefiles
import SetOfCurvesPlotter
import CreateOrnParameters
import numpy as np
import fit_orn_params
import pylab
from FigureCreator import plot_params_portrait

def generate_params_exp(x, a1, a2, exp):
    return a1 * x**exp + a2

def generate_params_exp(x, y_min, y_max, x_exp):
    """
    Returns A * x**(x_exp) + B but calculates A and B from the given y_min and y_max
    """
    x_max = np.max(x)
    x_min = np.min(x)
    B = y_min
    A = (y_max - y_max) / ((x_max - 1)**x_exp  - x_min**x_exp)
    return A * x**x_exp + B


def linear_transformation(x, y_min, y_max):
    """
    x : the range to be transformed
    y_min, y_max : lower and upper boundaries for the range into which x
    is transformed to
    Returns y = f(x), f(x) = m * x + b
    """
    x_min = np.min(x)
    x_max = np.max(x)
    if x_min == x_max:
        x_max = x_min * 1.0001
    return (y_min + (y_max - y_min) / (x_max - x_min) * (x - x_min))



class HandTuneOrnParameters(object):
    """
    Calculates for a set of response curves the use defined fitness
    """

    def __init__(self, params):
        self.params = params

    def set_parameters(self, argv):

        self.sim_cnt = int(argv[1])
        self.gor_min = float(argv[2])
        self.gor_max = float(argv[3])
        self.gkcag_min = float(argv[4])
        self.gkcag_max = float(argv[5])
        self.gcal_min = float(argv[6])
        self.gcal_max = float(argv[7])
        self.gleak_min = float(argv[8])
        self.gleak_max = float(argv[9])
        self.params['gor_min'] = self.gor_min
        self.params['gor_max'] = self.gor_max
        self.params['gor_exp'] = 3


    def set_orn_params(self):

        # class to write parameters into file
        OrnParamClass = CreateOrnParameters.CreateOrnParameters(self.params)
        OrnParamClass.param_list = ["gna", "gk", "gkcag", "gcal", "gleak_orn", "tau_cadec"]
        # define parameters:
        OrnParamClass.set_oor_value_for_conc_sweep()
        OrnParamClass.current_param_values = np.zeros((self.params["n_orn_x"], len(OrnParamClass.param_list)))
        # OrnParamClass.current_param_values[col][2] 
        # corresponds to the "gkcag" values for all ORNs in this type (i.e. giving one of the n_gor response curves)

        self.gor_values = OrnParamClass.gor_values # set based on self.params['gor_min, gor_max, gor_exp']
        self.gkcag_values = linear_transformation(self.gor_values, self.gkcag_min, self.gkcag_max)
        self.gcal_values = linear_transformation(self.gor_values, self.gcal_min, self.gcal_max)
        self.gleak_values = linear_transformation(self.gor_values, self.gleak_min, self.gleak_max)

        testparams = np.ones((self.params['n_gor'], 6))
        testparams[:, 0] *= 0.5  # g_na
        testparams[:, 1] *= 0.05 # g_k
        testparams[:, 2] = self.gkcag_values
        testparams[:, 3] = self.gcal_values
        testparams[:, 4] = self.gleak_values
        testparams[:, 5] *= 1000 # tau_ca_decay

        for i_ in xrange(n_test_curves):
            for param in xrange(len(OrnParamClass.param_list)):
                OrnParamClass.current_param_values[i_][param] = testparams[i_, param]

                
        # now overwrite gleak values according to the corresponding function
        param_fn = params['orn_params_fn_base'] + '%d.dat' % self.sim_cnt
        OrnParamClass.write_current_param_values_to_file(param_fn)
        log_file = open("%s/params_handtuning_%d.txt" % (params['params_folder'], self.sim_cnt), 'w')
        for i in xrange(params["n_gor"]):
            line = "%.1e\t%.1e\t%.1e\t%.1e\t%.1e\t%.1e\t%.1e\n" % ( \
                    OrnParamClass.gor_values[i],\
                    OrnParamClass.current_param_values[i][0],\
                    OrnParamClass.current_param_values[i][1],\
                    OrnParamClass.current_param_values[i][2],\
                    OrnParamClass.current_param_values[i][3],\
                    OrnParamClass.current_param_values[i][4],\
                    OrnParamClass.current_param_values[i][5])
            log_file.write(line)
            log_file.flush()


    def run_simulation(self):
        os.chdir('neuron_files') # this is important to avoide problems with tabchannel files and the functions defined therein
        pn = self.sim_cnt
        neuron_command = "mpirun -np %d $(which nrniv) -mpi -nobanner -nogui \
                -c \"x=%d\" -c \"strdef param_file\" -c \"sprint(param_file, \\\"%s\\\")\" start_file_epth_response_curve.hoc > delme%d" \
                % (params['n_proc'], pn, params['hoc_file'], pn)

        t1 = time.time()
        os.system("rm %s/*" % (params["spiketimes_folder"]))
        os.system("rm %s/*" % (params["volt_folder"]))
        t2 = time.time()
        os.system(neuron_command)
        print 'Duration %.1f seconds' % (t2 - t1)
        os.chdir('../') # move back to normal


    def get_response_curve(self):

        Merger = MergeSpikefiles.MergeSpikefiles(params)
        Merger.merge_epth_spiketimes_file(pattern=self.sim_cnt)
        Merger.merge_epth_nspike_files(pattern=self.sim_cnt)
        SOCP = SetOfCurvesPlotter.SetOfCurvesPlotter(params)

        # ------------ Plot the chose parameters in dependency of GID and the respective gor values
        #pylab.rcParams.update(plot_params_portrait)
        fig = pylab.figure(figsize=(14, 10))
        ax1 = fig.add_subplot(421)
        ax2 = fig.add_subplot(422)
        ax3 = fig.add_subplot(423)
        ax4 = fig.add_subplot(424)
        ax5 = fig.add_subplot(425)
        ax6 = fig.add_subplot(426)
        ax7 = fig.add_subplot(427)
        ax8 = fig.add_subplot(428)

        gids = np.arange(0, n_test_curves)
        ax1.plot(gids, self.gor_values)
        ax1.set_ylabel('g_or')
        ax3.plot(gids, self.gkcag_values)
        ax3.set_ylabel('g_k_cag')
        ax4.plot(self.gor_values, self.gkcag_values)
        ax4.set_xlabel('g_or')

        ax5.plot(gids, self.gcal_values)
        ax5.set_ylabel('g_k_cag')
        ax6.plot(self.gor_values, self.gcal_values)
        ax6.set_xlabel('g_or')

        ax7.plot(gids, self.gleak_values)
        ax7.set_ylabel('g_leak')
        ax8.plot(self.gor_values, self.gleak_values)
        ax8.set_xlabel('g_or')

        output_fn = params['figure_folder'] + '/hand_tuned_%d.png' % self.sim_cnt
        print 'Saving figure to:', output_fn
        x_data, y_data = SOCP.plot_set_of_curves(pn=self.sim_cnt, output_fn=output_fn)
        info_txt = 'gor: (%.1e, %.1e)\n' % (self.params['gor_min'], self.params['gor_min'])
        info_txt += 'gkcag: (%.1e, %.1e)\n' % (self.gkcag_min, self.gkcag_max)
        info_txt += 'gcal: (%.1e, %.1e)\n' % (self.gcal_min, self.gcal_max)
        info_txt += 'gleak: (%.1e, %.1e)\n' % (self.gleak_min, self.gleak_max)
        SOCP.annotate(info_txt)
        output_data = np.zeros((n_test_curves + 1, x_data[0].size))
        output_data[0, :] = x_data[0]
        output_data[1:, :] = y_data#.transpose()
        output_fn = params['other_folder'] + '/' + 'orn_response_curve_%d.dat' % (self.sim_cnt)
        print 'Saving data to:', output_fn
        np.savetxt(output_fn, output_data.transpose())


if __name__ == '__main__':

    param_tool = simulation_parameters.parameter_storage(use_abspath=True)
    params = param_tool.params

    n_test_curves = params['n_gor']
#    assert (n_test_curves == 10), 'Please set n_gor to the same number of test curves and parameter sets you provide'
    assert (params['rel_orn_mit'] == 1), 'Please set rel_orn_mit to 1'

    HTOP = HandTuneOrnParameters(params)
    HTOP.set_parameters(sys.argv)
    param_tool.hoc_export()

    HTOP.set_orn_params()
    HTOP.run_simulation()
    HTOP.get_response_curve()

#SOCP.plot_set_of_curves(output_fn=output_fn, pn=sim_cnt, ax=ax2)#, output_fn=output_fn)


#pylab.show()

#os.system('ristretto %s' % output_fn)



