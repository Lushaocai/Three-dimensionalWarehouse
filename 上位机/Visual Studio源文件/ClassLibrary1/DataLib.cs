using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Text;
using System.Threading.Tasks;

namespace DataLib
{
    //关联数据
   public class PlcData
    {
        //int 数据
        public int 仓库1数据;
        public int 仓库2数据;
        public int 仓库3数据;
        public int 灰色出库数量;
        public int 绿色出库数量;
        public int 蓝色出库数量;
        // bool 数据
        public bool 确认数量;
        public bool 出库模式;
        public bool 启动 = false;
        public bool 停止;
        public bool 正在停止;
        public bool 取消;
        public bool 出库启动;
        public bool 入库启动;
        public bool 运行标志位;
        public bool 停止中;
        public bool 仓库1货物进入传感器;
        public bool 仓库1货物离开传感器;

    }
    //历史数据
   

    //上升沿下降沿
    public class GetPN
    {
        bool _P = false;
        bool _N = false;

        public bool P(bool Value)
        {
            if (Value && !_P)
            {
                _P = true;
                return true;
            }

            if (!Value)
                _P = false;

            return false;
        }

        public bool N(bool Value)
        {
            if (!Value && _N)
            {
                _N = false;
                return true;
            }

            if (Value)
                _N = true;
            return false;
        }
    }

}
