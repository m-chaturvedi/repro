classdef PyMxRaw < handle
% Class representing a MATLAB extension of a Python object.

    properties (Access = protected)
        PyBaseCls
        PyBaseObj
    end
    
    methods
        function obj = PyMxRaw(pyBaseCls)
            obj.PyBaseCls = pyBaseCls;
            % Construct base proxy and pass object.
            % NOTE: Passing `feval` is just a cheap hack.
            % NOTE: Unwrap so that we can fiddle with values.
            pyCls = PyProxy.toPyValue(obj.PyBaseCls);
            pyFeval = PyProxy.toPyValue(@feval);
            mx_raw_obj = MexPyProxy.mx_to_mx_raw(obj);
            % Don't wrap this in a proxy just yet.
            obj.PyBaseObj = pyCls(mx_raw_obj, pyFeval);
        end

        function [varargout] = pyInvokeVirtual(obj, method, varargin)
            % TODO: Check method type, then invoke.
            % For now, just execute.
            assert(~strcmp(method, 'pyInvokeVirtual'));
            varargout = cell(1, nargout);
            [varargout{:}] = feval(method, obj, varargin{:});
        end
        
        function [varargout] = pyInvokeDirect(obj, method, varargin)
            assert(~strcmp(method, 'pyInvokeDirect'));
            varargout = cell(1, nargout);
            [varargout{:}] = obj.PyBaseObj.(method)(varargin{:});
        end
        
        % TODO: Make protected
        function [py] = pyObj(obj)
            py = obj.PyBaseObj;
        end
    end
end
